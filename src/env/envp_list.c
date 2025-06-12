/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:16:40 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/12 03:54:49 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

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
