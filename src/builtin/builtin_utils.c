/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/27 18:55:00 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

char	*find_equal_sign(const char *arg)
{
	char	*equality_sign;

	equality_sign = ft_strchr(arg, '=');
	if (equality_sign == NULL)
		return (NULL);
	return (equality_sign);
}
void	free_key_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
}