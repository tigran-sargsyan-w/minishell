/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:07:42 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 15:07:30 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

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
t_token	*lexer(char *input);

// token_utils.c
int		is_space(char c);
int		is_special(char c);
void	free_tokens(t_token *tokens);
void	add_token(t_token **tokens, t_token_type type, char *value,
			int separated);

// token_word.c
int		read_unquoted(t_token **tokens, char *input, int *i, int sep_flag);
int		read_squoted(t_token **tokens, char *input, int *i, int sep_flag);
int		read_dquoted(t_token **tokens, char *input, int *i, int sep_flag);

// token_operators.c
void	handle_pipe(t_token **tokens, int *i, int sep_flag);
void	handle_less(t_token **tokens, char *input, int *i, int sep_flag);
void	handle_greater(t_token **tokens, char *input, int *i, int sep_flag);

#endif