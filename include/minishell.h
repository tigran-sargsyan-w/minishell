/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:09:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/07 14:39:09 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtins.h"

typedef struct s_token		t_token;
typedef enum e_token_type	t_token_type;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

// lexer.c
t_token						*lexer(char *input);

// lexer_utils.c
int							is_space(char c);
int							is_special(char c);
void						add_token(t_token **tokens, t_token_type type,
								char *value);
void						print_tokens(t_token *tokens);
void						free_tokens(t_token *tokens);

// readline_loop.c
void						readline_loop(char **envp, 
								t_env_list **env_variables);

#endif
