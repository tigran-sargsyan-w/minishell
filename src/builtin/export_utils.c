/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:44:05 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 06:36:10 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int	export_without_args(t_env_list **env)
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
	return (0);
}

t_export_type	is_valid_export(char *arg, t_export_data *data)
{
	if ((ft_isalpha(arg[0]) != 1) && (arg[0] != '_'))
		return (0);
	arg++;
	while (*arg)
	{
		if ((*arg == '+') && (*(arg + 1) == '='))
		{
			data->type = CONCAT;
			return (1);
		}
		if (*arg == '=')
		{
			data->type = EXPORT;
			return (1);
		}
		if ((ft_isalnum(*arg) != 1) && (*arg != '_'))
			return (0);
		arg++;
	}
	data->type = NONE;
	return (1);
}

int	parse_key_value(char *arg, t_export_data *data)
{
	char	*plus;
	char	*equal;

	plus = ft_strchr(arg, '+');
	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		return (0);
	if (data->type == CONCAT)
		data->key = ft_substr(arg, 0, plus - arg);
	else
		data->key = ft_substr(arg, 0, equal - arg);
	if (data->key == NULL)
	{
		perror("minishell");
		return (1);
	}
	equal++;
	data->value = ft_strdup(equal);
	if (data->value == NULL)
	{
		perror("minishell");
		free(data->key);
		return (1);
	}
	return (0);
}

int	create_new_var(char *key, char *value, t_env_list **env)
{
	char		*new_key;
	char		*new_value;
	t_env_list	*node;

	new_key = ft_strdup(key);
	new_value = ft_strdup(value);
	if (new_key == NULL || new_value == NULL)
	{
		free(new_key);
		free(new_value);
		perror("minishell");
		return (1);
	}
	node = lst_create_node(new_key, new_value);
	if (node == NULL)
	{
		free(new_key);
		free(new_value);
		perror("minishell");
		return (1);
	}
	lst_add_end(env, node);
	return (0);
}
