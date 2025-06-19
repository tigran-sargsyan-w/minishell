/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 16:43:29 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Frees the old and new PWD strings.
 * This function is used to free the memory allocated for the old and new
 * PWD environment variables after they have been set or changed.
 * @param old_pwd Pointer to the old PWD string.
 * @param new_pwd Pointer to the new PWD string.
 */
void	free_pwds(char *old_pwd, char *new_pwd)
{
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
}

/**
 * @brief Frees the memory allocated for the export data.
 * This function is used to free the memory allocated for the key and value
 * in the export data structure.
 * @param data Pointer to the export data structure.
 */
void	free_export_data(t_export_data *data)
{
	free(data->key);
	free(data->value);
}

/**
 * @brief Safely duplicates a key-value pair into output_key and output_value.
 * This function allocates memory for the key and value, and handles errors
 * if memory allocation fails.
 * @param key The key to duplicate.
 * @param value The value to duplicate.
 * @param output_key Pointer to store the duplicated key.
 * @param output_value Pointer to store the duplicated value.
 * @return 0 on success, 1 on failure.
 */
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
