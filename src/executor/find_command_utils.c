/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 22:39:58 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 18:47:31 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * @brief Handles errors related to the `stat` system call.
 * This function checks if the command exists and is not a directory.
 * If an error occurs, it prints an appropriate error message.
 * @param cmd The command to check.
 * @param sh The shell context containing environment and state.
 */
void	handle_stat_errors(char *cmd, t_shell *sh)
{
	struct stat	sb;

	if (stat(cmd, &sb) == -1)
	{
		if (errno == ENOENT)
		{
			ft_dprintf(2, "minishell: %s: No such file or directory\n", cmd);
			sh->last_status = 127;
		}
		else
		{
			perror(cmd);
			sh->last_status = 126;
		}
		free_all_resources(sh);
		exit(sh->last_status);
	}
	else if (is_directory(cmd))
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", cmd);
		sh->last_status = 126;
		free_all_resources(sh);
		exit(sh->last_status);
	}
}
