/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:19:46 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/18 14:59:24 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			error_exit("open infile");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			error_exit("open outfile");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_child(t_cmd *cmd, t_shell *sh)
{
	char	*full_cmd;

	handle_input_redirection(cmd);
	handle_output_redirection(cmd);

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
