/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:16:40 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/07 17:42:35 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdlib.h>
#include "parser.h"

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
	t_env_list	*new_node;
	char		*equality_sign;
	char		*key;
	char		*value;
	int			key_len;

	list = NULL;
	while (*envp != NULL)
	{
		equality_sign = ft_strchr(*envp, '=');
		if (equality_sign == NULL)
		{
			envp++;
			continue ;
		}
		key_len = equality_sign - (*envp);
		key = ft_substr(*envp, 0, key_len);
		value = ft_strdup(++equality_sign);
		new_node = lst_create_node(key, value);
		if (key == NULL || value == NULL || new_node == NULL)
		{
			lst_clear(&list);
			return (NULL);
		}
		lst_add_end(&list, new_node);
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
