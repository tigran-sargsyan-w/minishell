/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:16:40 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:55:24 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>

/**
 * @brief Creates a new environment list node with the given key and value.
 * @param key The key of the environment variable.
 * @param value The value of the environment variable.
 * @return A pointer to the newly created node, or NULL on failure.
 */
t_env_list	*lst_create_node(char *key, char *value)
{
	t_env_list	*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

/**
 * @brief Adds a new environment node to the end of the list.
 * @param list Pointer to the pointer of the environment list.
 * @param new_node The new node to add to the list.
 */
void	lst_add_end(t_env_list **list, t_env_list *new_node)
{
	t_env_list	*tmp;

	if (list == NULL)
		return ;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	tmp = *list;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
}

/**
 * @brief Initializes the environment list from the given envp array.
 * @param envp The environment variables array.
 * @return A pointer to the initialized environment list, or NULL on failure.
 */
t_env_list	*lst_init(char *const *envp)
{
	t_env_list	*list;

	list = NULL;
	while (*envp)
	{
		if (add_env_entry(*envp, &list) == FAILURE)
		{
			lst_clear(&list);
			return (NULL);
		}
		envp++;
	}
	return (list);
}

/**
 * @brief Clears the environment list and frees all allocated memory.
 * @param list Pointer to the pointer of the environment list.
 */
void	lst_clear(t_env_list **list)
{
	t_env_list	*tmp;

	if (list == NULL || *list == NULL)
		return ;
	while (*list != NULL)
	{
		tmp = (*list)->next;
		if ((*list)->key != NULL)
			free((*list)->key);
		if ((*list)->value != NULL)
			free((*list)->value);
		free(*list);
		*list = tmp;
	}
}

/**
 * @brief Returns the size of the environment list.
 * @param list Pointer to the pointer of the environment list.
 * @return The number of nodes in the list, or 0 if the list is empty.
 */
size_t	lst_size(t_env_list **list)
{
	size_t		size;
	t_env_list	*tmp;

	if (list == NULL || *list == NULL)
		return (0);
	tmp = *list;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}
