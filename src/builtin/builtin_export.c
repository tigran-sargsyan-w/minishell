/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 00:41:09 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "ft_printf.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int	parse_key_value(char *arg, char **key, char **value)
{
	char	*plus_sign;
	char	*equality_sign;

	plus_sign = ft_strchr(arg, '+');
	equality_sign = ft_strchr(arg, '=');
	if (plus_sign && (plus_sign + 1 == equality_sign))
	{
		*key = ft_substr(arg, 0, plus_sign - arg);
		if (*key == NULL)
		{
			perror("minishell");
			return (1);
		}
		*value = ft_strdup(++equality_sign);
		if (*value == NULL)
		{
			perror("minishell");
			free(*key);
			return (1);
		}
	}
	else
	{
		*key = ft_substr(arg, 0, equality_sign - arg);
		if (*key == NULL)
		{
			perror("minishell");
			return (1);
		}
		*value = ft_strdup(++equality_sign);
		if (*value == NULL)
		{
			perror("minishell");
			free(*key);
			return (1);
		}
	}
	return (0);
}

int	export_argument(char *key, char *value, t_env_list **env,
		t_export_type type)
{
	t_env_list	*new_node;
	t_env_list	*tmp_node;
	char		*new_value;

	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	tmp_node = find_node_by_key(env, key);
	if (tmp_node)
	{
		if (type == EXPORT)
			set_value(tmp_node, value);
		else if (type == CONCAT)
		{
			new_value = ft_strjoin(tmp_node->value, value);
			if (!new_value)
			{
				perror("malloc");
				free_key_value(key, value);
				return (1);
			}
			if (!set_value(tmp_node, new_value))
			{
				free(new_value);
				free_key_value(key, value);
				return (1);
			}
			free(new_value);
		}
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
static int	export_or_concat(char *arg, t_export_data *data, t_env_list **env,
		t_export_type type)
{
	if (ft_strchr(arg, '='))
	{
		if (parse_key_value(arg, &data->key, &data->value) == 1)
			return (1);
		if (export_argument(data->key, data->value, env, type) != 0)
			return (1);
	}
	return (0);
}

static int	handle_argument(char *arg, t_export_data *data, t_env_list **env)
{
	t_export_type	type;

	type = is_valid_export(arg);
	if (type == ERROR)
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	else if (type == EXPORT || type == CONCAT)
		export_or_concat(arg, data, env, type);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	t_export_data	data;
	char			**argv;
	int				ret;

	ft_memset(&data, 0, sizeof(data));
	ret = 0;
	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	argv = &cmd->args[1];
	while (*argv)
	{
		ret = handle_argument(*argv, &data, env);
		argv++;
	}
	return (ret);
}
