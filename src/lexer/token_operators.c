/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:05:39 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 00:03:29 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	handle_pipe(t_token **tokens, int *i, int sep_flag)
{
	add_token(tokens, TOK_PIPE, "|", sep_flag);
	(*i)++;
}

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
