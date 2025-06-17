/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:11:19 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/17 21:01:38 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_cmd		t_cmd;
typedef struct s_env_list	t_env_list;
typedef struct s_shell		t_shell;

typedef enum e_export_type
{
	NONE,
	EXPORT,
	CONCAT
}							t_export_type;

typedef struct s_export_data
{
	char					*key;
	char					*value;
	t_export_type			type;
}							t_export_data;

// Built-ins
typedef int					(*t_builtin_func)(t_cmd *cmd, t_env_list **env);

typedef struct s_builtins_array
{
	char					*builtin_name;
	t_builtin_func			handler;
}							t_builtins_array;

// Builtin Functions
int							builtin_echo(t_cmd *cmd, t_env_list **env);
int							builtin_cd(t_cmd *cmd, t_env_list **env);
int							builtin_pwd(t_cmd *cmd, t_env_list **env);
int							builtin_env(t_cmd *cmd, t_env_list **env);
int							builtin_export(t_cmd *cmd, t_env_list **env);
int							builtin_unset(t_cmd *cmd, t_env_list **env);
int							builtin_exit(t_shell *sh, char **argv);
int							run_builtin(t_cmd *cmd, t_shell *sh);

// Utils

int							safe_strdup_pair(char *key, char *value,
								char **output_key, char **output_value);
void						free_pwds(char *old_pwd, char *new_pwd);

// Export utils
int							export_without_args(t_env_list **env);
int							export_argument(char *key, char *value,
								t_env_list **env);
void						free_key_value(char *key, char *value);
void						free_export_data(t_export_data *data);
t_export_type				is_valid_export(char *arg, t_export_data *data);
int							parse_key_value(char *arg, t_export_data *data);
int							create_new_var(char *key, char *value,
								t_env_list **env);

#endif
