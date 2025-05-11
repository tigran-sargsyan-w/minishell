/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/11 19:27:14 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h" // ft_strncmp, ft_strlcpy, ft_strlcat, ft_strdup
#include "parser.h"
#include <fcntl.h>     // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND
#include <stdio.h>     // perror
#include <stdlib.h>    // exit
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // waitpid
#include <unistd.h>    // fork, execve

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

void	execute_child(t_cmd *cmd, char **envp)
{
	char	*full_cmd;

	handle_input_redirection(cmd);
	handle_output_redirection(cmd);
	full_cmd = find_command(cmd->args[0], envp);
	if (!full_cmd)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 21);
		exit(CMD_NOT_FOUND);
	}
	execve(full_cmd, cmd->args, envp);
	perror("execve");
	free(full_cmd);
	exit(FAILURE);
}

void	execute_multiple_cmd(t_cmd *cmd, char **envp)
{
	t_pipe	pipefd;
	int		prev_pipe_read_fd;
	pid_t	pid;

	prev_pipe_read_fd = 0;
	while (cmd)
	{
		// Create a pipe for the next command
		if (cmd->next)
		{
			if (pipe(pipefd.fds) < 0)
				error_exit("pipe");
		}
		pid = fork();
		if (pid < 0)
			error_exit("fork");
		if (pid == 0)
		{
			// stdin ← предыдущий pipe (или стандартный, если первого)
			if (prev_pipe_read_fd != 0)
			{
				dup2(prev_pipe_read_fd, STDIN_FILENO);
				close(prev_pipe_read_fd);
			}
			// stdout → в текущий pipe (если есть следующая команда)
			if (cmd->next)
			{
				close(pipefd.read);
				dup2(pipefd.write, STDOUT_FILENO);
				close(pipefd.write);
			}
			execute_child(cmd, envp);
			exit(FAILURE);
		}
		// в родителе закрываем лишние дескрипторы
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
}

void	execute_single_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
	{
		execute_child(cmd, envp);
		exit(FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}
