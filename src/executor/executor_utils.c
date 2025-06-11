/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:19:46 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 11:54:25 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Вынесённая функция для обработки одной строки heredoc
static int	process_heredoc_line(char *line, int fd, t_redir *redir,
		t_shell *sh)
{
	char	*expanded;

	if (ft_strcmp(line, redir->filename) == 0)
	{
		free(line);
		return (0);
	}
	if (redir->quoted == 0)
	{
		expanded = expand_vars(line, sh);
		if(expanded != NULL)
			write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
	}
	write(fd, "\n", 1);
	free(line);
	return (1);
}

// Основная функция записи heredoc
static int	write_heredoc_content(t_redir *redir, t_shell *sh)
{
	int		fd;
	char	*line;

	fd = open(HEREDOC_TMPFILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open heredoc");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!process_heredoc_line(line, fd, redir, sh))
			break ;
	}
	close(fd);
	exit(0);
}

static int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	pid_t	pid;
	int		status;

	// 1. Ignore SIGINT in the parent process
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		// 2. In the child process — restore SIGINT
		signal(SIGINT, SIG_DFL);
		write_heredoc_content(redir, sh);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		// 3. Restore signals in the parent
		signal(SIGINT, sigint_handler);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			// Ctrl+C → interrupt the entire command
			unlink(HEREDOC_TMPFILE);
			sh->last_status = 130;
			return (-1);
		}
	}
	else
	{
		perror("fork");
		return (-1);
	}
	// 4. Replace heredoc with a regular infile
	free(redir->filename);
	redir->filename = ft_strdup(HEREDOC_TMPFILE);
	if (!redir->filename)
		return (-1);
	redir->type = REDIR_IN;
	return (0);
}

static int	apply_one_redir(t_redir *redir, t_shell *sh)
{
	int	fd;
	int	ret;

	fd = -1;
	if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc(redir, sh));
	// 2) open the file with the required mode
	if (redir->type == REDIR_IN)
	{
		fd = open(redir->filename, O_RDONLY);
	}
	else if (redir->type == REDIR_OUT)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (fd < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	// 3) duplicate the descriptor into stdin or stdout
	if (redir->type == REDIR_IN)
	{
		ret = dup2(fd, STDIN_FILENO);
	}
	else
	{
		ret = dup2(fd, STDOUT_FILENO);
	}
	if (ret < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_cmd *cmd, t_shell *sh)
{
	t_redir	*redir;

	redir = cmd->in_redirs;
	while (redir)
	{
		if (apply_one_redir(redir, sh) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = cmd->out_redirs;
	while (redir)
	{
		if (apply_one_redir(redir, sh) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

void	execute_child(t_cmd *cmd, t_shell *sh)
{
	char	*full_cmd;

	if (handle_redirections(cmd, sh) < 0)
	{
		sh->last_status = 1;
		exit(1);
	}
	// Handle the case: no command, only redirections
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (run_builtin(cmd, sh) == -1)
	{
		if (cmd->args[0] && cmd->args[0][0] == '\0')
		{
			write(2, "'': command not found\n", 23);
			exit(CMD_NOT_FOUND);
		}
		full_cmd = find_command(cmd->args[0], sh->env_tab);
		if (is_directory(full_cmd))
		{
			free(full_cmd);
			exit(CMD_IS_DIRECTORY);
		}
		if (!full_cmd)
		{
			write(2, cmd->args[0], ft_strlen(cmd->args[0]));
			write(2, ": command not found\n", 21);
			exit(CMD_NOT_FOUND);
		}
		execve(full_cmd, cmd->args, sh->env_tab);
		free(full_cmd);
		error_exit("execve");
	}
}

void	setup_child_fds(int prev_fd, t_pipe pd, t_cmd *cmd)
{
	if (prev_fd)
	{
		dup2(prev_fd, STDIN_FILENO);
		// TODO: security check for all dups and closes
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pd.read);
		dup2(pd.write, STDOUT_FILENO);
		close(pd.write);
	}
}

pid_t	fork_and_execute_cmd(t_cmd *cmd, t_shell *sh, int prev_fd, t_pipe pd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_exit("fork");
	// TODO: Change error exit (avoid exit call)
	if (pid == 0)
	{
		// In the child process — default signal handling
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		setup_child_fds(prev_fd, pd, cmd);
		execute_child(cmd, sh);
		exit(0);
	}
	return (pid);
}
