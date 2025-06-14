/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/14 18:25:04 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

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
