/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/27 19:19:27 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

static int	export_argument(char *key, char *value, t_env_list **env)
{
	t_env_list	*new_node;
	t_env_list	*tmp_node;

	tmp_node = find_node_by_key(env, key);
	if (tmp_node)
	{
		set_value(tmp_node, value);
		free_key_value(key, value);
	}
	else
	{
		if ((new_node = lst_create_node(key, value)) == NULL)
		{
			perror("malloc");
			free_key_value(key, value);
			return (1);
		}
		lst_add_end(env, new_node);
	}
	return (0);
}

// Check multiple args
int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*node;
	char		**argv;
	char		*equality_sign;
	char		*key;
	char		*value;

	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	argv = ++cmd->args;
	while (*argv)
	{
		if ((equality_sign = find_equal_sign(*argv)) == NULL)
		{
			argv++;
			continue ;
		}
		if ((key = ft_substr(*argv, 0, equality_sign - *argv)) == NULL)
		{
			perror("minishell");
			return (1);
		}
		value = ft_strdup(++equality_sign);
		if (value == NULL)
		{
			perror("minishell");
			free(key);
			return (1);
		}
		if ((node = find_node_by_key(env, key)) != NULL)
			set_value(node, value);
		export_argument(key, value, env);
		argv++;
	}
	return (0);
}
