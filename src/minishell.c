/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 17:43:05 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
