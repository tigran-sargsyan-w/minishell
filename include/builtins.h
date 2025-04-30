/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:11:19 by dsemenov          #+#    #+#             */
/*   Updated: 2025/04/30 22:16:59 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;

}						t_env_list;

t_env_list				*lst_create_node(char *key, char *value);
void					lst_add_end(t_env_list **list, t_env_list *new_node);
t_env_list				*lst_init(char **envp);
void					lst_clear(t_env_list **list);
#endif