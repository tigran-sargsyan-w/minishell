/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/22 19:50:48 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static t_env_list	*find_node_by_key(t_env_list **env, char *key)
{
	t_env_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static t_env_list	*set_value(t_env_list *node, char *value)
{
	free(node->value);
	node->value = ft_strdup(value);
	if (!node->value)
		return (NULL);
	return (node);
}

// READ MAN CD
int	builtin_cd(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*old_pwd;
	t_env_list	*new_pwd;

	old_pwd = find_node_by_key(env, "OLDPWD");
	new_pwd = find_node_by_key(env, "PWD");
	if (cmd->args[2] != NULL)
	{
		ft_putendl_fd("minishell: Too many arguments", 2);
		return (1);
	}
	else if (cmd->args[1] == NULL)
	{
		chdir(find_node_by_key(env, "HOME")->value);
		set_value(old_pwd, new_pwd->value);
		set_value(new_pwd, getcwd(NULL, 0));
		return (0);
	}
	if (chdir(cmd->args[1]) == -1)
	{
		perror("minishell");
		return (1);
	}
	else
	{
		set_value(old_pwd, new_pwd->value);
		set_value(new_pwd, getcwd(NULL, 0));
		return (0);
	}
	return (0);
}
