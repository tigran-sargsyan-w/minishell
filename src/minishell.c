/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/02 16:59:09 by dsemenov         ###   ########.fr       */
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
	char		*input_str;
	t_env_list	*env_variables;

	(void)argc;
	(void)argv;
	input_str = NULL;
	env_variables = lst_init(envp);
	if ((env_variables) == NULL)
	{
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return (1);
	}
	readline_loop(&input_str);
	lst_clear(&env_variables);
	return (0);
}
