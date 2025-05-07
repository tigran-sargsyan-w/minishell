/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/07 17:16:29 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv, char **envp)
{
	t_env_list	*env_list;

	(void)argc;
	(void)argv;
	env_list = lst_init(envp);
	if ((env_list) == NULL)
	{
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return (1);
	}
	readline_loop(envp, &env_list);
	lst_clear(&env_list);
	return (0);
}
