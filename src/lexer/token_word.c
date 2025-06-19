/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:05:37 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:46:22 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "lexer.h"
#include <stdlib.h>

/**
 * @brief Reads an unquoted word from the input and adds it as a token.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to read from.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 * @return Returns 0 on success, or 1 if an error occurs.
 */
int	read_unquoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int		start;
	char	*dup_word;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	dup_word = ft_strndup(input + start, *i - start);
	add_token(tokens, TOK_WORD, dup_word, sep_flag);
	free(dup_word);
	return (0);
}

/**
 * @brief Reads a single-quoted word from the input and adds it as a token.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to read from.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 * @return Returns 0 on success, or 1 if an error occurs.
 */
int	read_squoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int		start;
	char	*dup_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
	{
		ft_dprintf(2, "minishell: syntax error: unclosed single quote\n");
		return (1);
	}
	dup_word = ft_strndup(input + start, *i - start);
	add_token(tokens, TOK_SQUOTED, dup_word, sep_flag);
	(*i)++;
	free(dup_word);
	return (0);
}

/**
 * @brief Reads a double-quoted word from the input and adds it as a token.
 * @param tokens Pointer to the list of tokens.
 * @param input The input string to read from.
 * @param i Pointer to the current index in the input string.
 * @param sep_flag Indicates if the token is separated by whitespace.
 * @return Returns 0 on success, or 1 if an error occurs.
 */
int	read_dquoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int		start;
	char	*dup_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] != '"')
	{
		ft_dprintf(2, "minishell: syntax error: unclosed double quote\n");
		return (1);
	}
	dup_word = ft_strndup(input + start, *i - start);
	add_token(tokens, TOK_DQUOTED, dup_word, sep_flag);
	(*i)++;
	free(dup_word);
	return (0);
}
