/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:00:25 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/15 16:27:37 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>

static char	*ft_strjoin3(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*res;

	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = malloc(total_len + 1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcat(res, s1, total_len + 1);
    ft_strlcat(res, "=", 2);
	ft_strlcat(res, s2, total_len + 1);
	return (res);
}

char    **env_list_to_tab(t_env_list **env_list)
{
    char        **env_tab;
    t_env_list  *tmp;

    while (tmp)
    {
        
        tmp = tmp->next;
    }
    return (env_tab);
}