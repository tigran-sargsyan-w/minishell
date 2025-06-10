/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:00:25 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/10 21:08:00 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

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

void	free_env_tab(char **env_tab)
{
	char	**tmp;

	if (env_tab == NULL)
		return ;
	tmp = env_tab;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(env_tab);
}

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
