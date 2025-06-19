/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:33:07 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:40:41 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stddef.h>

/**
 * @brief Handles the tokenization of the input string 
 * based on the current character.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to tokenize.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 * @return Returns 1 if an error occurs, otherwise returns 0.
 */
static int	handle_token(t_token **tokens, char *input, int *i, int sep_flag)
{
	if (input[*i] == '|')
		handle_pipe(tokens, i, sep_flag);
	else if (input[*i] == '<')
		handle_less(tokens, input, i, sep_flag);
	else if (input[*i] == '>')
		handle_greater(tokens, input, i, sep_flag);
	else if (input[*i] == '\'')
	{
		if (read_squoted(tokens, input, i, sep_flag))
			return (1);
	}
	else if (input[*i] == '"')
	{
		if (read_dquoted(tokens, input, i, sep_flag))
			return (1);
	}
	else
	{
		if (read_unquoted(tokens, input, i, sep_flag))
			return (1);
	}
	return (0);
}

/**
 * @brief Tokenizes the input string into a list of tokens.
 * @param input The input string to tokenize.
 * @return Returns a pointer to the list of tokens, or NULL on error.
 */
t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;
	int		last_was_space;
	int		sep_flag;

	tokens = NULL;
	i = 0;
	sep_flag = 0;
	last_was_space = 1;
	while (input[i])
	{
		if (is_space(input[i]))
		{
			last_was_space = 1;
			i++;
			continue ;
		}
		sep_flag = last_was_space;
		if (handle_token(&tokens, input, &i, sep_flag))
			return (free_tokens(tokens), NULL);
		last_was_space = 0;
	}
	return (tokens);
}
