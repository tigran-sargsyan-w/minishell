/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:05:39 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:42:18 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief Handles the pipe operator tokenization.
 * @param tokens Pointer to the list of tokens.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 */
void	handle_pipe(t_token **tokens, int *i, int sep_flag)
{
	add_token(tokens, TOK_PIPE, "|", sep_flag);
	(*i)++;
}

/**
 * @brief Handles the less than operator tokenization.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to tokenize.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 */
void	handle_less(t_token **tokens, char *input, int *i, int sep_flag)
{
	if (input[*i + 1] == '<')
	{
		add_token(tokens, TOK_DLESS, "<<", sep_flag);
		*i += 2;
	}
	else
	{
		add_token(tokens, TOK_LESS, "<", sep_flag);
		(*i)++;
	}
}

/**
 * @brief Handles the greater than operator tokenization.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to tokenize.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 */
void	handle_greater(t_token **tokens, char *input, int *i, int sep_flag)
{
	if (input[*i + 1] == '>')
	{
		add_token(tokens, TOK_DGREATER, ">>", sep_flag);
		*i += 2;
	}
	else
	{
		add_token(tokens, TOK_GREATER, ">", sep_flag);
		(*i)++;
	}
}
