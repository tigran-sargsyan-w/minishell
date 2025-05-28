/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/28 23:25:28 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>

void	free_key_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
}

t_export_type	is_valid_name(char *arg)
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
t_unset_type	is_valid(char *arg)
{
	if ((ft_isalpha(arg[0]) != 1) && (arg[0] != '_'))
	{
		ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n", arg);
		return (INVALID);
	}
	arg++;
	while (*arg)
	{
		if ((ft_isalnum(*arg) != 1) && (*arg != '_'))
		{
			ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n", arg);
			return (INVALID);
		}
		arg++;
	}
	return (VALID);
}