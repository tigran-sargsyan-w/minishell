/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 16:41:50 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

void	free_pwds(char *old_pwd, char *new_pwd)
{
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
}

void	free_export_data(t_export_data *data)
{
	free(data->key);
	free(data->value);
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
