/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:07:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/12 03:15:01 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

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
	char	*dup;

	if (!node || !value)
		return (NULL);
	dup = ft_strdup(value);
	if (!dup)
	{
		perror("minishell");
		return (NULL);
	}
	if (node->value != value)
		free(node->value);
	node->value = dup;
	return (node);
}
