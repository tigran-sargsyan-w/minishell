/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/07 18:38:28 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include "libft.h"
#include <stdlib.h>

static int is_in_envlist(t_env_list **env, char *name, char *value)
{
	t_env_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, name) == 0)
		{
			tmp->value = ft_strdup(value);
			if (tmp->value == NULL)
			{
				perror("malloc");
				return (-1);
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*new_node;
	char		*equality_sign;
	char		*key;
	char		*value;
	int			key_len;
	
	if (env == NULL || *env == NULL)
		return (1);
	equality_sign = ft_strchr(cmd->args[1], '=');
	if (equality_sign == NULL)
	{
		ft_putendl_fd("export: not enough arguments", 2);
		return (1);
	}
	key_len = equality_sign - (cmd->args[1]);
	key = ft_substr(cmd->args[1], 0, key_len);
	value = ft_strdup(++equality_sign);
	if (key == NULL || value == NULL)
	{
		perror("malloc");
		return (1);
	}
	new_node = lst_create_node(key, value);
	if (new_node == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (is_in_envlist(env, key, value) == 0)
		lst_add_end(env, new_node);
	return (0);
}
