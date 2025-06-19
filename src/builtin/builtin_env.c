/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:59 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 16:25:48 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Prints the environment variables to standard output.
 *
 * @param cmd The command structure containing arguments.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 if there are too many arguments.
 */
int	builtin_env(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*tmp;

	if (cmd->args[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (env == NULL || *env == NULL)
		return (0);
	tmp = *env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
