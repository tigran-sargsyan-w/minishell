/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/06 19:19:59 by dsemenov         ###   ########.fr       */
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
		if (find_node_by_key(env, "HOME"))
		{
			chdir(find_node_by_key(env, "HOME")->value);
			set_value(old_pwd, new_pwd->value);
			set_value(new_pwd, getcwd(NULL, 0));
			return (0);
		}
		else
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
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
