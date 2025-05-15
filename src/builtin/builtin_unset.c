/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/15 16:15:09 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "parser.h"
#include "libft.h"
#include <stdlib.h>

int	builtin_unset(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*cur;
	t_env_list	*prev;

	if (cmd->args[1] == NULL)
	{
		ft_putendl_fd("unset: not enough arguments", 2);
		return (1);
	}
	if (env == NULL || *env == NULL)
		return (0);
	cur = *env;
	prev = NULL;
	while (cur && ft_strcmp(cur->key, cmd->args[1]) != 0)
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
