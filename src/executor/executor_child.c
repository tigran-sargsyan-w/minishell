/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:52 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 14:49:56 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include <signal.h>
#include <stdlib.h>

static void	execute_external_command(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_name;
	char	*full_cmd;

	cmd_name = cmd->args[0];
	if (cmd_name[0] == '\0')
	{
		write(2, "'': command not found\n", 23);
		exit(CMD_NOT_FOUND);
	}
	full_cmd = find_command(cmd_name, sh->env_tab);
	if (!full_cmd)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": command not found\n", 21);
		exit(CMD_NOT_FOUND);
	}
	if (is_directory(full_cmd))
	{
		free(full_cmd);
		exit(CMD_IS_DIRECTORY);
	}
	execve(full_cmd, cmd->args, sh->env_tab);
	free(full_cmd);
	error_exit("execve");
}

static void	execute_child(t_cmd *cmd, t_shell *sh)
{
	if (handle_redirections(cmd, sh) < 0)
	{
		sh->last_status = 1;
		exit(1);
	}
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (run_builtin(cmd, sh) == -1)
		execute_external_command(cmd, sh);
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

pid_t	fork_and_execute_cmd(t_cmd *cmd, t_shell *sh, int prev_fd, t_pipe pd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		setup_child_fds(prev_fd, pd, cmd);
		execute_child(cmd, sh);
		exit(0);
	}
	return (pid);
}
