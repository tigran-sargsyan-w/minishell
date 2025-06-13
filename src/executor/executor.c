/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 01:22:15 by dsemenov         ###   ########.fr       */
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
		//TODO: free char ** env?
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

static void	execute_cmds(t_cmd *cmd, t_shell *sh)
{
	t_pipe	pipefd;
	int		prev_fd;
	pid_t	last_pid;
	t_cmd	*cur;

	prev_fd = 0;
	cur = cmd;
	while (cur)
	{
		if (cur->next)
			if (pipe(pipefd.fds) < 0)
				error_exit("pipe");
		last_pid = fork_and_execute_cmd(cur, cmd, sh, prev_fd, pipefd);
		if (prev_fd != 0)
			close(prev_fd);
		if (cur->next)
		{
			close(pipefd.write);
			prev_fd = pipefd.read;
		}
		cur = cur->next;
	}
	finalize_execution(last_pid, sh);
}

// Function for a single command (without pipes)
static void	run_single_command(t_cmd *current_cmd, t_cmd *cmd, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		error_exit("dup stdin");
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout < 0)
		error_exit("dup stdout");
	if (handle_redirections(current_cmd, cmd, sh) < 0)
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
		run_single_command(cmd, cmd, sh);
	else
		execute_cmds(cmd, sh);
	free_env_tab(sh->env_tab);
}
