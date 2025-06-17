/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 06:34:21 by dsemenov         ###   ########.fr       */
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

int	concat_argument(char *key, char *value, t_env_list **env)
{
	t_env_list	*node;
	char		*new_value;

	node = find_node_by_key(env, key);
	if (node == NULL)
		return (export_argument(key, value, env));
	if (node->value != NULL)
		new_value = ft_strjoin(node->value, value);
	else
		new_value = ft_strjoin("", value);
	if (new_value == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (set_value(node, new_value) == NULL)
	{
		free(new_value);
		return (1);
	}
	free(new_value);
	return (0);
}

int	export_argument(char *key, char *value, t_env_list **env)
{
	t_env_list	*node;

	node = find_node_by_key(env, key);
	if (node != NULL)
	{
		if (set_value(node, value) == NULL)
			return (1);
		return (0);
	}
	return (create_new_var(key, value, env));
}

static int	export_or_concat(t_export_data *data, t_env_list **env)
{
	if (data->type == NONE)
		return (0);
	if (data->type == EXPORT)
		return (export_argument(data->key, data->value, env));
	return (concat_argument(data->key, data->value, env));
}

static int	handle_export_arg(char *arg, t_env_list **env)
{
	t_export_data	data;
	int				err;

	ft_memset(&data, 0, sizeof(data));
	if (!is_valid_export(arg, &data))
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	err = parse_key_value(arg, &data);
	if (err == 0)
		err = export_or_concat(&data, env);
	free_export_data(&data);
	return (err);
}

int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	char	**argv;
	int		ret;

	ret = 0;
	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	argv = &cmd->args[1];
	while (*argv != NULL)
	{
		if (handle_export_arg(*argv, env))
			ret = 1;
		argv++;
	}
	return (ret);
}
