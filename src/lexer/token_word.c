/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:05:37 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 19:43:04 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "minishell.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int	read_unquoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int	start;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	add_token(tokens, TOK_WORD, ft_strndup(input + start, *i - start),
		sep_flag);
	return (0);
}

int	read_squoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int	start;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
	{
		ft_dprintf(2, "minishell: syntax error: unclosed single quote\n");
		return (1);
	}
	add_token(tokens, TOK_SQUOTED, ft_strndup(input + start, *i - start),
		sep_flag);
	(*i)++;
	return (0);
}

int	read_dquoted(t_token **tokens, char *input, int *i, int sep_flag)
{
	int	start;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] != '"')
	{
		ft_dprintf(2, "minishell: syntax error: unclosed double quote\n");
		return (1);
	}
	add_token(tokens, TOK_DQUOTED, ft_strndup(input + start, *i - start),
		sep_flag);
	(*i)++;
	return (0);
}
