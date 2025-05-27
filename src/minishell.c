/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/27 19:33:09 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>

static t_shell	*init_sh(t_shell *sh, char **envp)
{
	sh->env_list = lst_init(envp);
	sh->last_status = 0;
	if (sh->env_list == NULL)
	{
		perror("minishell: init env");
		return (NULL);
	}
	return (sh);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	ft_memset(&sh, 0, sizeof(sh));
	if (init_sh(&sh, envp) == NULL)
		return (1);
	readline_loop(&sh);
	lst_clear(&sh.env_list);
	return (sh.last_status);
}
