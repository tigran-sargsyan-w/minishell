/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/22 17:04:35 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "builtins.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_cmds(t_cmd *cmd, t_shell *sh)
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
		fork_and_execute_cmd(cmd, sh, prev_pipe_read_fd, pipefd);
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
	unlink(HEREDOC_TMPFILE);
}

void	executor(t_cmd *cmd, t_shell *sh)
{
	int		saved_stdin;
	int		saved_stdout;

	sh->env_tab = env_list_to_tab(&sh->env_list);
	// TODO: Handle NULL tab
	//if (!sh->env_tab)
	if (cmd->next == NULL)
	{
		// Single command with possible redirection
		saved_stdin = dup(STDIN_FILENO);
		// TODO: check -1 return for dup
		saved_stdout = dup(STDOUT_FILENO);
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		// Run builtin (or fall back to external)
		if (run_builtin(cmd, sh) == -1)
			execute_cmds(cmd, sh);
		// Restore original fds
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		// TODO: check if closed
		close(saved_stdout);
	}
	else
		execute_cmds(cmd, sh);
	free_cmd_list(cmd);
	free_env_tab(sh->env_tab);
}
