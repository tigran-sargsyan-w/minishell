/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 01:48:11 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "executor.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

void	execute_cmds(t_cmd *cmd, t_shell *sh)
{
	t_pipe	pipefd;
	int		prev_pipe_read_fd;
	int		status;
	pid_t	pid;

	prev_pipe_read_fd = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipefd.fds) < 0)
				error_exit("pipe");
		}
		pid = fork_and_execute_cmd(cmd, sh, prev_pipe_read_fd, pipefd);
		if (prev_pipe_read_fd != 0)
			close(prev_pipe_read_fd);
		if (cmd->next)
		{
			close(pipefd.write);
			prev_pipe_read_fd = pipefd.read;
		}
		cmd = cmd->next;
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (pid < 0)
	{
		lst_clear(&sh->env_list);
		error_exit("fork");
		return ;
	}
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
	unlink(HEREDOC_TMPFILE);
	while (wait(&status) > 0)
		;
}

void	executor(t_cmd *cmd, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;

	sh->env_tab = env_list_to_tab(&sh->env_list);
	// TODO: Handle NULL tab
	// if (!sh->env_tab)
	if (cmd->next == NULL)
	{
		// Single command with possible redirection
		saved_stdin = dup(STDIN_FILENO);
		// TODO: check -1 return for dup
		saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirections(cmd,sh) < 0)
		{
			sh->last_status = 1;
			free_env_tab(sh->env_tab);
			return ;
		}
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
	// free_cmd_list(cmd); TODO: Fix double free crash (export TMP=HELLO)
	free_env_tab(sh->env_tab);
}
