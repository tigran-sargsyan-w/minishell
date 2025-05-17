/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/17 10:03:42 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "builtins.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_cmds(t_cmd *cmd, char **envp, t_shell *sh)
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
		fork_and_execute_cmd(cmd, envp, prev_pipe_read_fd, pipefd, &sh->env_list);
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

void	executor(t_cmd *cmd, t_shell *sh)
{
	int		saved_stdin;
	int		saved_stdout;
	char	**envp;

	envp = env_list_to_tab(&sh->env_list);
	if (cmd->next == NULL)
	{
		// Single command with possible redirection
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		// Run builtin (or fall back to external)
		if (run_builtin(cmd, &sh->env_list) == -1)
			execute_cmds(cmd, envp, sh);
		// Restore original fds
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	else
		execute_cmds(cmd, envp, sh);
	free_cmd_list(cmd);
	free_env_tab(envp);
}