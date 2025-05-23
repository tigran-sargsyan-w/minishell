/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:19:46 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/23 19:58:25 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static int	handle_heredoc(const char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || !strcmp(line, limiter))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	// duplicate the end of the pipe into stdin
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("dup2 heredoc");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

static int	apply_one_redir(t_redir *redir)
{
	int	fd;
	int	ret;

	if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc(redir->filename));
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

int	handle_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->in_redirs;
	while (redir)
	{
		if (apply_one_redir(redir) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = cmd->out_redirs;
	while (redir)
	{
		if (apply_one_redir(redir) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

void	execute_child(t_cmd *cmd, t_shell *sh)
{
	char	*full_cmd;

	handle_redirections(cmd);
	if (run_builtin(cmd, sh) == -1)
	{
		full_cmd = find_command(cmd->args[0], sh->env_tab);
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
		//TODO: security check for all dups and closes
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pd.read);
		dup2(pd.write, STDOUT_FILENO);
		close(pd.write);
	}
}

void	fork_and_execute_cmd(t_cmd *cmd, t_shell *sh, int prev_fd, t_pipe pd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_exit("fork");
	// TODO: Change error exit (avoid exit call)
	if (pid == 0)
	{
		setup_child_fds(prev_fd, pd, cmd);
		execute_child(cmd, sh);
		exit(0);
	}
}
