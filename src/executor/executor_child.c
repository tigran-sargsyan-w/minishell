/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:52 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 10:59:55 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "ft_printf.h"
#include <signal.h>
#include <stdlib.h>

static void	handle_cmd_not_found(t_shell *sh, char *cmd_name, int exit_code);
static void	execute_external_command(t_cmd *cmd, t_shell *sh);
static void	execute_child(t_cmd *current_cmd, t_shell *sh);
static void	setup_child_fds(int prev_fd, t_pipe pd, t_cmd *cmd);

/**
 * @brief Handles the case when a command is not found.
 * This function prints an error message
 * and exits the shell with the specified exit code.
 * @param sh The shell structure containing environment and status.
 * @param cmd_name The name of the command that was not found.
 * @param exit_code The exit code to use when exiting the shell.
 */
static void	handle_cmd_not_found(t_shell *sh, char *cmd_name, int exit_code)
{
	ft_dprintf(2, "%s: command not found\n", cmd_name);
	free_all_resources(sh);
	sh->last_status = exit_code;
	exit(exit_code);
}

/**
 * @brief Executes an external command by finding its full path
 * and calling execve with the command's arguments.
 * If the command is not found, it handles the error appropriately.
 * @param cmd The command structure containing the command and its arguments.
 * @param sh The shell structure containing environment variables.
 */
static void	execute_external_command(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_name;
	char	*full_cmd;

	cmd_name = cmd->args[0];
	if (cmd_name[0] == '\0')
		handle_cmd_not_found(sh, "''", CMD_NOT_FOUND);
	full_cmd = find_command(cmd_name, sh);
	if (!full_cmd)
		handle_cmd_not_found(sh, cmd_name, CMD_NOT_FOUND);
	close_all_fds();
	execve(full_cmd, cmd->args, sh->env_tab);
	free_all_resources(sh);
	free(full_cmd);
	error_exit("execve");
}

/**
 * @brief Executes a command in a child process.
 * This function handles redirections, checks for built-in commands,
 * and executes external commands if necessary.
 * It also handles the case where no arguments are provided.
 * @param current_cmd The command to execute.
 * @param sh The shell structure containing environment variables and status.
 */
static void	execute_child(t_cmd *current_cmd, t_shell *sh)
{
	if (handle_redirections(current_cmd) < 0)
	{
		free_all_resources(sh);
		sh->last_status = 1;
		exit(sh->last_status);
	}
	if (!current_cmd->args || !current_cmd->args[0])
	{
		free_all_resources(sh);
		exit(0);
	}
	if (run_builtin(current_cmd, sh) == -1)
		execute_external_command(current_cmd, sh);
}

/**
 * @brief Sets up the file descriptors for a child process.
 * This function redirects the standard input and output
 * based on the previous command's file descriptor and the current pipe.
 * It also closes the appropriate file descriptors to avoid leaks.
 * @param prev_fd The file descriptor from the previous command (if any).
 * @param pd The pipe structure containing read and write file descriptors.
 * @param cmd The current command being executed.
 */
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

/**
 * @brief Forks a new process to execute a command.
 * This function sets up the child process's file descriptors,
 * handles signals, and executes the command in the child process.
 * It returns the process ID of the child.
 * @param current_cmd The command to execute in the child process.
 * @param sh The shell structure containing environment variables and status.
 * @param prev_fd The file descriptor from the previous command (if any).
 * @param pd The pipe structure for inter-process communication.
 * @return The process ID of the child process.
 */
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
