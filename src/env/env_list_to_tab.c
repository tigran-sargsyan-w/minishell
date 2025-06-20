/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:00:25 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:51:36 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static char	*ft_strjoin3(char const *s1, char const *s2);
static char	**return_empty_tab(void);
static char	**fill_tab(t_env_list **list, char **tab, size_t len);

/**
 * @brief Joins two strings with an '=' in between.
 * Allocates memory for the new string and returns it.
 * @param s1 The first string (key).
 * @param s2 The second string (value).
 * @return A new string containing "s1=s2", or NULL on failure.
 */
static char	*ft_strjoin3(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*res;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 2;
	res = malloc(total_len + 1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcat(res, s1, total_len + 1);
	ft_strlcat(res, "=", total_len + 1);
	ft_strlcat(res, s2, total_len + 1);
	return (res);
}

/**
 * @brief Returns an empty environment tab.
 * Allocates memory for a single NULL pointer and returns it.
 * @return A pointer to an array containing a single NULL pointer,
 * or NULL on failure.
 */
static char	**return_empty_tab(void)
{
	char	**tab;

	tab = malloc(sizeof(char *));
	if (tab == NULL)
	{
		perror("minishell:");
		return (NULL);
	}
	*tab = NULL;
	return (tab);
}

/**
 * @brief Fills the environment tab with key-value pairs from the list.
 * @param list Pointer to the environment list.
 * @param tab Pointer to the environment tab.
 * @param len The number of elements to fill.
 * @return A pointer to the filled environment tab, or NULL on failure.
 */
static char	**fill_tab(t_env_list **list, char **tab, size_t len)
{
	t_env_list	*tmp;
	size_t		i;

	tmp = *list;
	i = 0;
	while (tmp && i < len)
	{
		tab[i] = ft_strjoin3(tmp->key, tmp->value);
		if (tab[i] == NULL)
		{
			free_env_tab(tab);
			perror("minishell");
			return (NULL);
		}
		tmp = tmp->next;
		i++;
	}
	return (tab);
}

/**
 * @brief Converts an environment list to a tab of strings.
 * This function iterates through the environment list,
 * concatenates each key-value pair into a string of the form "key=value",
 * and stores the result in a newly allocated array.
 * @param env_list Pointer to the environment list.
 * @return A pointer to an array of strings representing the environment,
 * or NULL on failure. If the list is empty, returns an array with a single
 * NULL pointer.
 */
char	**env_list_to_tab(t_env_list **env_list)
{
	char	**env_tab;
	size_t	len;

	len = lst_size(env_list);
	if ((!env_list) || (len == 0))
		return (return_empty_tab());
	env_tab = malloc(sizeof(char *) * (len + 1));
	if (env_tab == NULL)
	{
		perror("minishell");
		return (NULL);
	}
	env_tab[len] = NULL;
	env_tab = fill_tab(env_list, env_tab, len);
	return (env_tab);
}
