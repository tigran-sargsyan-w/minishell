/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:09:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 18:56:42 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127
# define CMD_IS_DIRECTORY 126
# define HEREDOC_TMPFILE "minishell_heredoc"

typedef struct s_env_list	t_env_list;
typedef struct s_cmd		t_cmd;

typedef struct s_shell
{
	t_env_list				*env_list;
	char					**env_tab;
	int						last_status;
	t_cmd					*cmd_list;
}							t_shell;

void						readline_loop(t_shell *sh);
int							is_directory(const char *path);

// free_utils.c
void						free_cmd_list(t_cmd *cmd);
void						free_all_resources(t_shell *sh);
void						free_env_tab(char **env_tab);
void						free_all_env(t_shell *sh);

#endif
