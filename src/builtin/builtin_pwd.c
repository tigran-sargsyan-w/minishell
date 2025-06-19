/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:23 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 16:38:22 by tsargsya         ###   ########.fr       */
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

/**
 * @brief Built-in command to print the current working directory.
 * This function handles the 'pwd' command in the shell.
 * It attempts to get the current working directory using getcwd,
 * and if that fails, it tries to find the PWD environment variable.
 * @param cmd The command structure containing the arguments.
 * @param env Pointer to the environment list.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
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
