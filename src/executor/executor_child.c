/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:52 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/17 06:21:11 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "ft_printf.h"
#include "libft.h"
#include "minishell.h"
#include <signal.h>
#include <stdlib.h>

static void	handle_cmd_not_found(t_shell *sh, char *cmd_name, int exit_code)
{
	ft_dprintf(2, "%s: command not found\n", cmd_name);
	free_all_resources(sh);
	sh->last_status = exit_code;
	exit(exit_code);
}

static void	execute_external_command(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_name;
	char	*full_cmd;

	cmd_name = cmd->args[0];
	if (cmd_name[0] == '\0')
		handle_cmd_not_found(sh, "''", CMD_NOT_FOUND);
	full_cmd = find_command(cmd_name, sh->env_tab);
	if (!full_cmd)
		handle_cmd_not_found(sh, cmd_name, CMD_NOT_FOUND);
	if (is_directory(full_cmd))
	{
		ft_dprintf(2, "%s: Is a directory\n", cmd_name);
		sh->last_status = CMD_IS_DIRECTORY;
		free_all_resources(sh);
		free(full_cmd);
		exit(sh->last_status);
	}
	execve(full_cmd, cmd->args, sh->env_tab);
	free_all_resources(sh);
	free(full_cmd);
	error_exit("execve");
}

static void	execute_child(t_cmd *current_cmd, t_shell *sh)
{
	if (handle_redirections(current_cmd, sh) < 0)
	{
		free_all_resources(sh);
		sh->last_status = 1;
		exit(1);
	}
	if (!current_cmd->args || !current_cmd->args[0])
	{
		free_all_resources(sh);
		exit(0);
	}
	if (run_builtin(current_cmd, sh) == -1)
		execute_external_command(current_cmd, sh);
}

static void	setup_child_fds(int prev_fd, t_pipe pd, t_cmd *cmd)
{
	int	dup2_ret;

	if (prev_fd)
	{
		dup2_ret = dup2(prev_fd, STDIN_FILENO);
		if (dup2_ret < 0)
			error_exit("dup2");
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pd.read);
		dup2_ret = dup2(pd.write, STDOUT_FILENO);
		if (dup2_ret < 0)
			error_exit("dup2");
		close(pd.write);
	}
}

pid_t	fork_and_execute_cmd(t_cmd *current_cmd, t_shell *sh, int prev_fd,
		t_pipe pd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		setup_child_fds(prev_fd, pd, current_cmd);
		execute_child(current_cmd, sh);
		free_all_resources(sh);
		exit(sh->last_status);
	}
	return (pid);
}
