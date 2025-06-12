/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/12 02:46:35 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

void	free_key_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
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

int	safe_strdup_pair(char *key, char *value, char **output_key,
		char **output_value)
{
	*output_key = ft_strdup(key);
	if (!*output_key)
		return (perror("minishell"), 1);
	*output_value = ft_strdup(value);
	if (!*output_value)
	{
		free(*output_key);
		perror("minishell");
		return (1);
	}
	return (0);
}
