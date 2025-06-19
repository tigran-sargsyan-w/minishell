/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 16:39:52 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

/**
 * @brief Removes a variable from the environment list.
 * @param name The name of the variable to remove.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	remove_var(char *name, t_env_list **env)
{
	t_env_list	*cur;
	t_env_list	*prev;

	cur = *env;
	prev = NULL;
	while (cur && ft_strcmp(cur->key, name) != 0)
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur)
	{
		if (prev)
			prev->next = cur->next;
		else
			*env = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		return (0);
	}
	return (0);
}

/**
 * @brief Built-in command to unset environment variables.
 * This function handles the 'unset' command in the shell.
 * It removes the specified environment variables from the environment list.
 * @param cmd The command structure containing the arguments.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	builtin_unset(t_cmd *cmd, t_env_list **env)
{
	char	**argv;

	argv = &cmd->args[1];
	if (*argv == NULL)
		return (0);
	if (env == NULL || *env == NULL)
		return (0);
	while (*argv)
	{
		remove_var(*argv, env);
		argv++;
	}
	return (0);
}
