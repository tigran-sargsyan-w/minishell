/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/09 18:01:22 by tsargsya         ###   ########.fr       */
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
		{
			perror("minishell: open infile");
			exit(1);
		}
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
		{
			perror("minishell: open outfile");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_pipeline(t_cmd *cmd, char **envp, t_env_list **env_vars)
{
	t_pipe	pipefd;
	int		in_fd;
	pid_t	pid;
	// int		fds[2];

	(void)env_vars;
	in_fd = 0;
	while (cmd)
	{
		// Если есть следующая команда — создаём новый pipe
		if (cmd->next)
		{
			if (pipe((int*)&pipefd) < 0)
			{
				perror("pipe");
				exit(1);
			}
			// pipefd.read = fds[0];
			// pipefd.write = fds[1];
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			// stdin ← предыдущий pipe (или стандартный, если первого)
			if (in_fd != 0)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			// stdout → в текущий pipe (если есть следующая команда)
			if (cmd->next)
			{
				close(pipefd.read);
				dup2(pipefd.write, STDOUT_FILENO);
				close(pipefd.write);
			}
			// редиректы < и >
			handle_input_redirection(cmd);
			handle_output_redirection(cmd);
			// выполнить одну команду (без fork внутри)
			execute_cmd(cmd, envp);
			// на всякий случай, если execve не сработал:
			exit(0);
		}
		// в родителе закрываем лишние дескрипторы
		if (in_fd != 0)
			close(in_fd);
		if (cmd->next)
		{
			close(pipefd.write);
			in_fd = pipefd.read;
		}
		cmd = cmd->next;
	}
	// ждём всех дочерних процессов
	while (wait(NULL) > 0)
		;
}

void	execute_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*full_cmd;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		full_cmd = find_command(cmd->args[0], envp);
		if (!full_cmd)
		{
			write(2, "minishell: ", 11);
			write(2, cmd->args[0], ft_strlen(cmd->args[0]));
			write(2, ": command not found\n", 21);
			exit(127);
		}
		execve(full_cmd, cmd->args, envp);
		perror("execve");
		free(full_cmd);
		exit(127);
	}
	else
		waitpid(pid, &status, 0);
}
