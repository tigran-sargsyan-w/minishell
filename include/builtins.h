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

// Linked list for environment variables
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

// Built-ins
typedef int (*t_builtin_func)(int argc, char **argv, t_env_list *env);

typedef struct s_builtins_array
{
	char *builtin_name;
	t_builtin_func handler;	
} t_builtins_array;

// Builtin Functions
int builtin_echo(int argc, char **argv, t_env_list *env);
int builtin_cd(int argc, char **argv, t_env_list *env);
int builtin_pwd(int argc, char **argv, t_env_list *env);
int builtin_env(int argc, char **argv, t_env_list *env);
int builtin_export(int argc, char **argv, t_env_list *env);
int builtin_unset(int argc, char **argv, t_env_list *env);
int builtin_exit(int argc, char **argv, t_env_list *env);

#endif