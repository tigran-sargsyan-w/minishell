/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:44:05 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 00:41:32 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include <stdio.h>

int	export_without_args(t_env_list **env)
{
	t_env_list	*tmp;

	if (env == NULL || *env == NULL)
		return (1);
	tmp = *env;
	while (tmp)
	{
		printf("declare -x %s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

t_export_type	is_valid_export(char *arg)
{
	if ((ft_isalpha(arg[0]) != 1) && (arg[0] != '_'))
		return (ERROR);
	arg++;
	while (*arg)
	{
		if ((*arg == '+') && (*(arg + 1) == '='))
			return (CONCAT);
		if (*arg == '=')
			return (EXPORT);
		if ((ft_isalnum(*arg) != 1) && (*arg != '_'))
			return (ERROR);
		arg++;
	}
	return (EXPORT);
}
