/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:52 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 17:50:17 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include "ft_printf.h"
#include "env.h"
#include <signal.h>
#include <stdlib.h>

static void	execute_external_command(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_name;
	char	*full_cmd;

	cmd_name = cmd->args[0];
	if (cmd_name[0] == '\0')
	{
		ft_dprintf(2, "'': command not found\n");
		exit(CMD_NOT_FOUND);
	}
	full_cmd = find_command(cmd_name, sh->env_tab);
	if (!full_cmd)
	{
		ft_dprintf(2, "%s: command not found\n", cmd_name);
		free_all_env(sh);
		exit(CMD_NOT_FOUND);
	}
	if (is_directory(full_cmd))
	{
		free_all_env(sh);
		free(full_cmd);
		exit(CMD_IS_DIRECTORY);
	}
	execve(full_cmd, cmd->args, sh->env_tab);
	//TODO: free env if execve fails?
	free(full_cmd);
	error_exit("execve");
}

static void	execute_child(t_cmd *current_cmd, t_shell *sh)
{
	if (handle_redirections(current_cmd, sh) < 0)
	{
		free_all_env(sh);
		free_cmd_list(sh->cmd_list);
		sh->last_status = 1;
		exit(1);
	}
	if (!current_cmd->args || !current_cmd->args[0])
	{
		free_all_env(sh);
		free_cmd_list(sh->cmd_list);
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

pid_t	fork_and_execute_cmd(t_cmd *current_cmd, t_shell *sh, int prev_fd, t_pipe pd)
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
		free_cmd_list(sh->cmd_list);
		lst_clear(&sh->env_list);
		free_env_tab(sh->env_tab);
		exit(0);
	}
	return (pid);
}
