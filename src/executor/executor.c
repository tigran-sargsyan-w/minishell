/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:38:33 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/06 14:42:50 by tsargsya         ###   ########.fr       */
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
