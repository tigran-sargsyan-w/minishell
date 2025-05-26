/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:33:07 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/24 12:43:05 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	read_unquoted(t_token **tokens, char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_special(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	add_token(tokens, TOK_WORD, ft_strndup(input + start, *i - start));
	return (0);
}

static void	handle_pipe(t_token **tokens, int *i)
{
	add_token(tokens, TOK_PIPE, "|");
	(*i)++;
}

static void	handle_less(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		add_token(tokens, TOK_DLESS, "<<");
		*i += 2;
	}
	else
	{
		add_token(tokens, TOK_LESS, "<");
		(*i)++;
	}
}

static void	handle_greater(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		add_token(tokens, TOK_DGREATER, ">>");
		*i += 2;
	}
	else
	{
		add_token(tokens, TOK_GREATER, ">");
		(*i)++;
	}
}

static int	read_squoted(t_token **tokens, char *input, int *i)
{
	int	start;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
	{
		// TODO: handle unclosed single quote
		return (1);
	}
	add_token(tokens, TOK_SQUOTED, ft_strndup(input + start, *i - start));
	(*i)++;
	return (0);
}

static int	read_dquoted(t_token **tokens, char *input, int *i)
{
	int	start;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] != '"')
	{
		// TODO: handle unclosed double quote
		return (1);
	}
	add_token(tokens, TOK_DQUOTED, ft_strndup(input + start, *i - start));
	(*i)++;
	return (0);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (input[i] == '|')
			handle_pipe(&tokens, &i);
		else if (input[i] == '<')
			handle_less(&tokens, input, &i);
		else if (input[i] == '>')
			handle_greater(&tokens, input, &i);
		else if (input[i] == '\'')
		{
			if (read_squoted(&tokens, input, &i))
				return (free_tokens(tokens), NULL);
		}
		else if (input[i] == '"')
		{
			if (read_dquoted(&tokens, input, &i))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (read_unquoted(&tokens, input, &i))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
