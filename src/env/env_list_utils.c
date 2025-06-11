/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:07:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/10 05:37:15 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

t_env_list	*find_node_by_key(t_env_list **env, char *key)
{
	t_env_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_env_list	*set_value(t_env_list *node, char *value)
{
	char	*new_value;

	if (!node || !value)
		return (NULL);
	new_value = ft_strdup(value);
	if (!new_value)
	{
		perror("minishell");
		return (NULL);
	}
	free(node->value);
	node->value = new_value;
	return (node);
}
