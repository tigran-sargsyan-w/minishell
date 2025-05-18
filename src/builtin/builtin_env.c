/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:59 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/18 15:07:23 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	builtin_env(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*tmp;

	if (env == NULL || *env == NULL)
		return (2);
	if (cmd->args[1])
	{
		ft_putendl_fd("env: Env doesn't take any arguments or options",
			STDERR_FILENO);
		return (1);
	}
	tmp = *env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		//TODO: security check
		tmp = tmp->next;
	}
	return (0);
}
