/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/27 23:31:35 by dsemenov         ###   ########.fr       */
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

int	is_valid_name(char *arg)
{
	if ((ft_isalpha(arg[0]) != 1) && (arg[0] != '_'))
		return (0);
	arg++;
	while (*arg)
	{
		if (*arg == '=')
			return (1);
		if ((ft_isalnum(*arg) != 1) && (*arg != '_'))
			return (0);
		arg++;
	}
	return (1);
}