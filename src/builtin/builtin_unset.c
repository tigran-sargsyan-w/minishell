/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/28 23:29:14 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include "parser.h"
#include "libft.h"
#include <stdlib.h>

int remove_var(char *argv, t_env_list **env)
{
	t_env_list	*cur;
	t_env_list	*prev;
	
	cur = *env;
	prev = NULL;
	while (cur && ft_strcmp(cur->key, argv) != 0)
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

//Check multiple and use is_valid in the loop
int	builtin_unset(t_cmd *cmd, t_env_list **env)
{
	char **argv;

	argv = &cmd->args[1];
	if (argv == NULL)
		return (0);
	if (env == NULL || *env == NULL)
		return (1);
	while (*argv)
	{
		if (is_valid_unset(*argv) == VALID)
			remove_var(*argv, env);
		argv++;
	}
	return (0);
}
