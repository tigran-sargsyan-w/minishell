/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:09:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 21:01:43 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 0
# define FAILURE 1

typedef struct s_env_list	t_env_list;

typedef struct s_shell
{
	t_env_list				*env_list;
	char					**env_tab;
	int						last_status;
}							t_shell;

void						readline_loop(t_shell *sh);
int							is_directory(const char *path);

void						clean_shell(t_shell *sh);

#endif
