/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:33:07 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/31 20:21:27 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	read_unquoted(t_token **tokens, char *input, int *i, int sep_flag)
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

static void	handle_pipe(t_token **tokens, int *i, int sep_flag)
{
	add_token(tokens, TOK_PIPE, "|", sep_flag);
	(*i)++;
}

static void	handle_less(t_token **tokens, char *input, int *i, int sep_flag)
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

static void	handle_greater(t_token **tokens, char *input, int *i, int sep_flag)
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

static int	read_squoted(t_token **tokens, char *input, int *i, int sep_flag)
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
	add_token(tokens, TOK_SQUOTED, ft_strndup(input + start, *i - start),
		sep_flag);
	(*i)++;
	return (0);
}

static int	read_dquoted(t_token **tokens, char *input, int *i, int sep_flag)
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
	add_token(tokens, TOK_DQUOTED, ft_strndup(input + start, *i - start),
		sep_flag);
	(*i)++;
	return (0);
}

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
		}
		else
		{
			sep_flag = last_was_space;
			if (input[i] == '|')
			{
				handle_pipe(&tokens, &i, sep_flag);
				last_was_space = 0;
			}
			else if (input[i] == '<')
			{
				handle_less(&tokens, input, &i, sep_flag);
				last_was_space = 0;
			}
			else if (input[i] == '>')
			{
				handle_greater(&tokens, input, &i, sep_flag);
				last_was_space = 0;
			}
			else if (input[i] == '\'')
			{
				if (read_squoted(&tokens, input, &i, sep_flag))
					return (free_tokens(tokens), NULL);
				last_was_space = 0;
			}
			else if (input[i] == '"')
			{
				if (read_dquoted(&tokens, input, &i, sep_flag))
					return (free_tokens(tokens), NULL);
				last_was_space = 0;
			}
			else
			{
				if (read_unquoted(&tokens, input, &i, sep_flag))
					return (free_tokens(tokens), NULL);
				last_was_space = 0;
			}
		}
	}
	return (tokens);
}
