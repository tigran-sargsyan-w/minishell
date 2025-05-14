/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:59 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/14 19:51:00 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int	builtin_env(t_cmd *cmd, t_env_list **env)
{
	(void)cmd;
	t_env_list	*tmp;
	
	if (env == NULL || *env == NULL)
		return (1);
	if (cmd->args[1])
	{
		//TODO: Print correct error message - env: ‘example_arg’: No such file or directory
		return (127);
	}
	tmp = *env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
