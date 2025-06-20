/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:07:38 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:52:56 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	entry_fail(char *key, char *value);

/**
 * @brief Handles memory cleanups for failed entry additions.
 * @param key The key to free, if allocated.
 * @param value The value to free, if allocated.
 * @return 1 to indicate failure.
 */
static int	entry_fail(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
	perror("minishell");
	return (1);
}

/**
 * @brief Finds a node in the environment list by its key.
 * @param env Pointer to the environment list.
 * @param key The key to search for.
 * @return A pointer to the node with the matching key, or NULL if not found.
 */
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

/**
 * @brief Sets the value of a node in the environment list.
 * @param node The node to update.
 * @param value The new value to set.
 * @return A pointer to the updated node, or NULL on failure.
 */
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

/**
 * @brief Adds a new entry to the environment list.
 * @param entry The entry string in the format "key=value".
 * @param list Pointer to the environment list.
 * @return 0 on success, or 1 on failure.
 */
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
