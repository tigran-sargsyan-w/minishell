/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/12 13:55:40 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "ft_printf.h"
#include <signal.h>
#include <sys/wait.h>

static void	finalize_execution(pid_t last_pid, t_shell *sh)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &status, 0);
	if (last_pid < 0)
	{
		lst_clear(&sh->env_list);
		error_exit("fork");
	}
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
	unlink(HEREDOC_TMPFILE);
	while (wait(&status) > 0)
		;
}

// Main function for executing commands in a pipeline
static void	execute_cmds(t_cmd *cmd, t_shell *sh)
{
	t_pipe	pipefd;
	int		prev_pipe_read_fd;
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
	finalize_execution(pid, sh);
}

// Function for a single command (without pipes)
static void	run_single_command(t_cmd *cmd, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		error_exit("dup stdin");
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout < 0)
		error_exit("dup stdout");
	if (handle_redirections(cmd, sh) < 0)
	{
		sh->last_status = 1;
		return ;
	}
	if (run_builtin(cmd, sh) == -1)
		execute_cmds(cmd, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	executor(t_cmd *cmd, t_shell *sh)
{
	sh->env_tab = env_list_to_tab(&sh->env_list);
	if (!sh->env_tab)
	{
		ft_dprintf(2, "minishell: env_list_to_tab failed\n");
		sh->last_status = FAILURE;
		return ;
	}
	if (cmd->next == NULL)
		run_single_command(cmd, sh);
	else
		execute_cmds(cmd, sh);
	free_cmd_list(cmd);
	free_env_tab(sh->env_tab);
}
