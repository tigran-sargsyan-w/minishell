/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/07 17:46:38 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>

int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	(void)cmd;
	t_env_list	*tmp;
	
	if (env == NULL || *env == NULL)
		return (1);
	if (cmd->args == NULL)
	{
		tmp = *env;
		while (tmp)
		{
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			tmp = tmp->next;
		}
		return (0);
	}
	tmp = *env;
	while (tmp)
	{
		lst_create_node(tmp->key, tmp->value);
	}
	return (0);
}
