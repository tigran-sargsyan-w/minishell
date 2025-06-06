/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:19:46 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/06 18:35:10 by tsargsya         ###   ########.fr       */
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
#include <unistd.h>

static int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	int		fd;
	char	*line;
	char	*expanded;

	// 1) create/truncate tmp file for heredoc
	fd = open(HEREDOC_TMPFILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open heredoc tmpfile");
		return (-1);
	}
	// 2) read lines until matching the limiter
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break ;
		}
		if (redir->quoted == 0)
		{
			expanded = expand_vars(line, sh);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
		{
			write(fd, line, ft_strlen(line));
		}
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	// 3) change the node: from heredoc to a regular infile
	free(redir->filename);
	redir->filename = ft_strdup(HEREDOC_TMPFILE);
	if (!redir->filename)
	{
		perror("strdup");
		return (-1);
	}
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
	if (run_builtin(cmd, sh) == -1)
	{
		full_cmd = find_command(cmd->args[0], sh->env_tab);
		// TODO: Handle checking if cmd is directory
		// Validate if cmd is dir return 126
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
