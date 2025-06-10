/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:09:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 18:16:51 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# define SUCCESS 0
# define FAILURE 1

typedef struct s_env_list	t_env_list;

// Main structure
typedef struct s_shell
{
	t_env_list				*env_list;
	char					**env_tab;
	int						last_status;
}							t_shell;

// readline_loop.c
void						readline_loop(t_shell *sh);
bool						is_directory(const char *path);

#endif
