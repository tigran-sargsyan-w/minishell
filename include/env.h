/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:03:21 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/27 14:10:56 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stddef.h>

// Linked list for environment variables
typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;

}						t_env_list;

t_env_list				*lst_init(char *const *envp);
void					lst_clear(t_env_list **list);
t_env_list				*lst_create_node(char *key, char *value);
void					lst_add_end(t_env_list **list, t_env_list *new_node);
size_t					lst_size(t_env_list **list);
char					**env_list_to_tab(t_env_list **env_list);
void					free_env_tab(char **env_tab);
t_env_list				*find_node_by_key(t_env_list **env, char *key);
t_env_list				*set_value(t_env_list *node, char *value);

#endif
