/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/15 16:14:44 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int	is_in_envlist(t_env_list **env, char *name, char *value)
{
	t_env_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, name) == 0)
		{
			free(tmp->value);
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

static int	export_without_args(t_env_list **env)
{
	t_env_list	*tmp;

	if (env == NULL || *env == NULL)
		return (1);
	tmp = *env;
	while (tmp)
	{
		printf("declare -x %s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

static char *find_equal_sign(const char *arg)
{
	char		*equality_sign;

	equality_sign = ft_strchr(arg, '=');
	if (equality_sign == NULL)
		return (NULL);
	return (equality_sign);
}

int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*new_node;
	char 		*equality_sign;
	char		*key;
	char		*value;
	int			ret;

	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	if ((equality_sign = find_equal_sign(cmd->args[1])) == NULL)
		return (0);
	key = ft_substr(cmd->args[1], 0, equality_sign - cmd->args[1]);
	value = ft_strdup(++equality_sign);
	if (key == NULL || value == NULL)
	{
		perror("malloc");
		free(key);
		free(value);
		return (1);
	}
	ret = is_in_envlist(env, key, value);
	if (ret < 0)
	{
		free(key);
		free(value);
		return (1);
	}
	else if (ret == 1)
	{
		free(key);
		free(value);
		return (0);
	}
	if ((new_node = lst_create_node(key, value)) == NULL)
	{
		perror("malloc");
		free(key);
		free(value);
		return (1);
	}
	lst_add_end(env, new_node);
	return (0);
}
