/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:07:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 21:12:58 by dsemenov         ###   ########.fr       */
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
	free(node->value);
	node->value = dup;
	return (node);
}

static int	entry_fail(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
	perror("minishell");
	return (1);
}

int	add_env_entry(char *entry, t_env_list **list)
{
	char		*equal_sign;
	char		*key;
	char		*value;
	t_env_list	*node;

	equal_sign = ft_strchr(entry, '=');
	if (!equal_sign)
		return (0);
	key = ft_substr(entry, 0, equal_sign - entry);
	if (!key)
		return (entry_fail(NULL, NULL));
	value = ft_strdup(equal_sign + 1);
	if (!value)
		return (entry_fail(key, NULL));
	node = lst_create_node(key, value);
	if (!node)
		return (entry_fail(key, value));
	lst_add_end(list, node);
	return (0);
}
