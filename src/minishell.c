/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/15 16:10:09 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	t_env_list	*env_list;

	(void)argc;
	(void)argv;
	env_list = lst_init(envp);
	if (env_list == NULL)
	{
		perror("minishell: init env");
		return (1);
	}
	//TODO Pass only env_list in readline loop
	readline_loop(envp, &env_list);
	lst_clear(&env_list);
	return (0);
}
