/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:11:19 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/07 14:21:36 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_cmd	t_cmd;


// Linked list for environment variables
typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;

}						t_env_list;

t_env_list				**lst_init(char *const *envp);
void					lst_clear(t_env_list **list);

// Built-ins
typedef int				(*t_builtin_func)(t_cmd *cmd, t_env_list **env);

typedef struct s_builtins_array
{
	char				*builtin_name;
	t_builtin_func		handler;
}						t_builtins_array;

// Builtin Functions
int						builtin_echo(t_cmd *cmd, t_env_list **env);
int						builtin_cd(t_cmd *cmd, t_env_list **env);
int						builtin_pwd(t_cmd *cmd, t_env_list **env);
int						builtin_env(t_cmd *cmd, t_env_list **env);
int						builtin_export(t_cmd *cmd, t_env_list **env);
int						builtin_unset(t_cmd *cmd, t_env_list **env);
int						builtin_exit(t_cmd *cmd, t_env_list **env);
int						run_builtin(t_cmd *cmd, t_env_list **env);

#endif