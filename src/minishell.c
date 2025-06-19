/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 23:21:41 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes the shell structure with environment variables.
 * It sets the last status to 0, initializes the environment list,
 * and sets the command list to NULL.
 * @param sh Pointer to the shell structure to initialize.
 * @param envp The environment variables passed to the shell.
 * @return Returns SUCCESS (0) on success, FAILURE (1) on error.
 */
static int	init_sh(t_shell *sh, char **envp)
{
	ft_memset(sh, 0, sizeof(*sh));
	sh->last_status = 0;
	sh->env_list = lst_init(envp);
	if (sh->env_list == NULL && *envp != NULL)
	{
		perror("minishell: init env");
		return (FAILURE);
	}
	sh->cmd_list = NULL;
	return (SUCCESS);
}

/**
 * @brief The main function of the minishell program.
 * @param argc The number of command-line arguments (not used).
 * @param argv The command-line arguments (not used).
 * @param envp The environment variables passed to the shell.
 * @return Returns the last status of the shell, which is set by 
 * built-in commands or external commands executed during the session.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	if (init_sh(&sh, envp) == FAILURE)
		return (EXIT_FAILURE);
	readline_loop(&sh);
	lst_clear(&sh.env_list);
	return (sh.last_status);
}
