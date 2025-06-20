/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 11:03:35 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "ft_printf.h"
#include <signal.h>
#include <sys/wait.h>

static int	preprocess_heredocs(t_cmd *cmd_list, t_shell *sh);
static void	finalize_execution(pid_t last_pid, t_shell *sh);
static void	execute_cmds(t_cmd *cmd, t_shell *sh);
static void	run_single_command(t_cmd *cmd, t_shell *sh);

/**
 * @brief Preprocess heredocs in the command list.
 * This function iterates through the command list and processes each heredoc
 * redirection. It reads the content of each heredoc and writes it to a temporary
 * file.
 * @param cmd_list The list of commands to process.
 * @param sh The shell context containing environment and state.
 * @return 0 on success, -1 on failure.
 */
static int	preprocess_heredocs(t_cmd *cmd_list, t_shell *sh)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->in_redirs;
		while (r)
		{
			if (r->type == REDIR_HEREDOC)
				if (handle_heredoc(r, sh) < 0)
					return (-1);
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

/**
 * @brief Finalizes the execution of commands.
 * This function waits for the last child process to finish,
 * checks its exit status, and cleans up resources.
 * @param last_pid The PID of the last executed command.
 * @param sh The shell context containing environment and state.
 */
static void	finalize_execution(pid_t last_pid, t_shell *sh)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &status, 0);
	if (last_pid < 0)
	{
		free_all_resources(sh);
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

/**
 * @brief Executes a list of commands in a pipeline.
 * This function sets up pipes between commands and manages their execution.
 * @param cmd The list of commands to execute.
 * @param sh The shell context containing environment and state.
 */
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
		last_pid = fork_and_execute_cmd(cur, sh, prev_fd, pipefd);
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

/**
 * @brief Runs a single command in the shell.
 * This function handles redirections, executes built-in commands,
 * or forks a new process to execute external commands.
 * @param cmd The command to run.
 * @param sh The shell context containing environment and state.
 */
static void	run_single_command(t_cmd *cmd, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	redir_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
		error_exit("dup");
	redir_status = handle_redirections(cmd);
	if (redir_status >= 0 && cmd->args && cmd->args[0])
	{
		if (run_builtin(cmd, sh) == -1)
			execute_cmds(cmd, sh);
	}
	else
	{
		if (sh->last_status == 0)
			sh->last_status = 1;
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Executes commands in the shell.
 * This function initializes the environment, processes heredocs,
 * and executes either a single command or a list of commands.
 * @param sh The shell context containing environment and state.
 */
void	executor(t_shell *sh)
{
	sh->env_tab = env_list_to_tab(&sh->env_list);
	if (!sh->env_tab)
	{
		ft_dprintf(2, "minishell: env_list_to_tab failed\n");
		sh->last_status = FAILURE;
		return ;
	}
	if (preprocess_heredocs(sh->cmd_list, sh) < 0)
	{
		free_env_tab(sh->env_tab);
		return ;
	}
	if (sh->cmd_list->next == NULL)
		run_single_command(sh->cmd_list, sh);
	else
		execute_cmds(sh->cmd_list, sh);
	free_env_tab(sh->env_tab);
}
