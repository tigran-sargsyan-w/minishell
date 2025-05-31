/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:09:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/31 20:10:36 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_env_list	t_env_list;

// Main structure
typedef struct s_shell
{
	t_env_list				*env_list;
	char					**env_tab;
	int						last_status;
}							t_shell;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_SQUOTED,
	TOK_DQUOTED,
	TOK_PIPE,
	TOK_LESS,
	TOK_GREATER,
	TOK_DLESS,
	TOK_DGREATER
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	int						separated;
	struct s_token			*next;
}							t_token;

// lexer.c
t_token						*lexer(char *input);

// lexer_utils.c
int							is_space(char c);
int							is_special(char c);
void						add_token(t_token **tokens, t_token_type type,
								char *value, int separated);
void						print_tokens(t_token *tokens);
void						free_tokens(t_token *tokens);

// readline_loop.c
typedef struct s_env_list	t_env_list;
void						readline_loop(t_shell *sh);

#endif
