/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/12 10:30:49 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_multiple_cmd(t_cmd *cmd, char **envp)
{
	t_pipe	pipefd;
	int		prev_pipe_read_fd;

	prev_pipe_read_fd = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipefd.fds) < 0)
				error_exit("pipe");
		}
		fork_and_execute_cmd(cmd, envp, prev_pipe_read_fd, pipefd);
		if (prev_pipe_read_fd != 0)
			close(prev_pipe_read_fd);
		if (cmd->next)
		{
			close(pipefd.write);
			prev_pipe_read_fd = pipefd.read;
		}
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		continue ;
}

void	execute_single_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
	{
		execute_child(cmd, envp);
		error_exit("execve");
	}
	else
		waitpid(pid, &status, 0);
}
