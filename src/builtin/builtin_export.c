/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/26 03:05:48 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// static int	is_in_envlist(t_env_list **env, char *name, char *value)
// {
// 	t_env_list	*tmp;

// 	tmp = *env;
// 	while (tmp)
// 	{
// 		if (ft_strcmp(tmp->key, name) == 0)
// 		{
// 			free(tmp->value);
// 			tmp->value = ft_strdup(value);
// 			if (tmp->value == NULL)
// 			{
// 				perror("malloc");
// 				return (-1);
// 			}
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

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

static int handle_args(t_cmd *cmd, t_env_list **env)
{
	// int	ret;
	// t_env_list	*new_node;
	char	*equality_sign;
	char	*key;
	char	*value;
	(void) env;
	equality_sign = NULL;
	(cmd->args)++; 
	while (*cmd->args != NULL)
	{
		printf("%s\n", *cmd->args);
		if ((equality_sign = ft_strchr(*cmd->args, '=')) == NULL)
			break;
		key = ft_substr(*cmd->args, 0, equality_sign - *cmd->args);
		value = ft_strdup(++equality_sign);
		// if (key == NULL || value == NULL)
		// {
		// 	perror("malloc");
		// 	free(key);
		// 	free(value);
		// 	ret = 1;
		// 	//continue;
		// }
		// ret = is_in_envlist(env, key, value);
		// if (ret < 0)
		// {
		// 	free(key);
		// 	free(value);
		// 	ret = 1;
		// 	//continue;
		// }
		// else if (ret == 1)
		// {
		// 	free(key);
		// 	free(value);
		// 	ret = 0;
		// 	//continue;
		// }
		// if ((new_node = lst_create_node(key, value)) == NULL)
		// {
		// 	perror("malloc");
		// 	free(key);
		// 	free(value);
		// 	ret = 1;
		// 	//continue;
		// }
		// lst_add_end(env, new_node);
		(cmd->args)++;
	}
	return (0);
}


//Check multiple args
int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	return (handle_args(cmd, env));
}
