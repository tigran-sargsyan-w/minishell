/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/07 12:51:46 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h" // ft_strncmp, ft_strlcpy, ft_strlcat, ft_strdup
#include "parser.h"
#include <stdio.h>     // perror
#include <stdlib.h>    // exit
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // waitpid
#include <unistd.h>    // fork, execve
#include <fcntl.h>     // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND

void	handle_redirections(t_cmd *cmd)
{
	int	fd;

	// Input redirection: <
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
	// Output redirection: > or >>
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
		handle_redirections(cmd);
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
