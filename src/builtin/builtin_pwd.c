/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:23 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 21:26:54 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "ft_printf.h"
#include "parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	builtin_pwd(t_cmd *cmd, t_env_list **env)
{
	char		*cwd;
	t_env_list	*pwd_node;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (EXIT_SUCCESS);
	}
	pwd_node = find_node_by_key(env, "PWD");
	if (pwd_node && pwd_node->value)
	{
		ft_dprintf(2, "pwd: warning: cannot access current directory: %s\n",
			strerror(errno));
		printf("%s\n", pwd_node->value);
		return (EXIT_SUCCESS);
	}
	perror("pwd");
	return (EXIT_FAILURE);
}
