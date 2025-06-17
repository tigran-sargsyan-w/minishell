/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 02:21:46 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	update_var(t_env_list **env, char *key, char *value)
{
	t_env_list	*node;
	char		*temp_key;
	char		*temp_value;

	node = find_node_by_key(env, key);
	if (node)
	{
		if (!set_value(node, value))
			return (1);
		return (0);
	}
	if (safe_strdup_pair(key, value, &temp_key, &temp_value))
		return (1);
	if (export_argument(temp_key, temp_value, env))
	{
		free(temp_key);
		free(temp_value);
		return (1);
	}
	return (0);
}

static int	update_pwd_pair(t_env_list **env, char *old, char *new)
{
	if (update_var(env, "OLDPWD", old))
		return (1);
	if (update_var(env, "PWD", new))
		return (1);
	return (0);
}

static int	get_target_path(t_cmd *cmd, t_env_list **env, char **dst)
{
	t_env_list	*home;

	if (cmd->args[1] == NULL)
	{
		home = find_node_by_key(env, "HOME");
		if (!home || !home->value)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		*dst = ft_strdup(home->value);
	}
	else if (cmd->args[2] != NULL)
	{
		ft_putendl_fd("minishell: cd: Too many arguments", 2);
		return (1);
	}
	else
		*dst = ft_strdup(cmd->args[1]);
	if (!*dst)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

static int	perform_cd(char *target, char **old_pwd, char **new_pwd)
{
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
	{
		perror("minishell");
		return (1);
	}
	if (chdir(target) == -1)
	{
		perror("minishell");
		free(*old_pwd);
		return (1);
	}
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
	{
		perror("minishell");
		free(*old_pwd);
		return (1);
	}
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_env_list **env)
{
	char	*target;
	char	*old_pwd;
	char	*new_pwd;

	if (get_target_path(cmd, env, &target))
		return (1);
	if (perform_cd(target, &old_pwd, &new_pwd))
	{
		free(target);
		return (1);
	}
	free(target);
	if (update_pwd_pair(env, old_pwd, new_pwd))
	{
		free(old_pwd);
		free(new_pwd);
		return (1);
	}
	free(old_pwd);
	free(new_pwd);
	return (0);
}
