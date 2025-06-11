/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/11 05:49:05 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	update_env_var(t_env_list **env, const char *key, char *value)
{
	t_env_list	*node;

	node = find_node_by_key(env, (char *)key);
	if (node)
	{
		if (!set_value(node, value))
		{
			free(value);
			perror("minishell");
			return (1);
		}
		free(value);
	}
	else
	{
		if (export_argument(ft_strdup(key), value, env, EXPORT) != 0)
		{
			free(value);
			return (1);
		}
	}
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_env_list **env)
{
	char		*old_pwd;
	char		*new_pwd;
	char		*target;
	t_env_list	*home_node;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (perror("minishell"), 1);
	if (cmd->args[1] == NULL)
	{
		home_node = find_node_by_key(env, "HOME");
		if (!home_node || !home_node->value)
			return (free(old_pwd), ft_putendl_fd("minishell: cd: HOME not set",
					2), 1);
		target = ft_strdup(home_node->value);
	}
	else if (cmd->args[2] != NULL)
		return (free(old_pwd),
			ft_putendl_fd("minishell: cd: Too many arguments", 2), 1);
	else
		target = ft_strdup(cmd->args[1]);
	if (!target)
		return (free(old_pwd), perror("minishell"), 1);
	if (chdir(target) < 0)
		return (perror("minishell"), free(old_pwd), free(target), 1);
	free(target);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("minishell"), free(old_pwd), 1);
	if (update_env_var(env, "OLDPWD", old_pwd))
		return (1);
	if (update_env_var(env, "PWD", new_pwd))
		return (1);
	return (0);
}
