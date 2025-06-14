/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:03:21 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/14 20:20:58 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# define SUCCESS 0
# define FAILURE 1
# include <stddef.h>

typedef struct s_shell	t_shell;

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
t_env_list				*find_node_by_key(t_env_list **env, char *key);
t_env_list				*set_value(t_env_list *node, char *value);
int						add_env_entry(char *entry, t_env_list **list);

#endif
