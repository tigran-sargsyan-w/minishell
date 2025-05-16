/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/16 17:49:37 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "builtins.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_cmds(t_cmd *cmd, char **envp, t_env_list **env_variables)
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
		fork_and_execute_cmd(cmd, envp, prev_pipe_read_fd, pipefd,env_variables);
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

void	executor(t_cmd *cmd, t_env_list **env_variables)
{
	int		saved_stdin;
	int		saved_stdout;
	char	**envp;

	envp = env_list_to_tab(env_variables);
	if (cmd->next == NULL)
	{
		// Single command with possible redirection
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		// Run builtin (or fall back to external)
		if (run_builtin(cmd, env_variables) == -1)
			execute_cmds(cmd, envp, env_variables);
		// Restore original fds
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	else
		execute_cmds(cmd, envp, env_variables);
	free_cmd_list(cmd);
	free_env_tab(envp);
}