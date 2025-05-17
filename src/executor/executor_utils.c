/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:19:46 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/17 10:08:14 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			error_exit("open infile");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			error_exit("open outfile");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_child(t_cmd *cmd, char **envp, t_env_list **env_variables)
{
	char	*full_cmd;

	handle_input_redirection(cmd);
	handle_output_redirection(cmd);

	if (run_builtin(cmd, env_variables) == -1)
	{
		full_cmd = find_command(cmd->args[0], envp);
		if (!full_cmd)
		{
			write(2, cmd->args[0], ft_strlen(cmd->args[0]));
			write(2, ": command not found\n", 21);
			exit(CMD_NOT_FOUND);
		}
		execve(full_cmd, cmd->args, envp);
		free(full_cmd);
		error_exit("execve");
	}
}

void	setup_child_fds(int prev_fd, t_pipe pd, t_cmd *cmd)
{
	if (prev_fd)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pd.read);
		dup2(pd.write, STDOUT_FILENO);
		close(pd.write);
	}
}

void	fork_and_execute_cmd(t_cmd *cmd, char **envp, int prev_fd, t_pipe pd, t_shell *sh)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
	{
		setup_child_fds(prev_fd, pd, cmd);
		execute_child(cmd, envp, &sh->env_list);
		exit(0);
	}
}
