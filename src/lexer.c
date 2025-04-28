/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:33:07 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 20:50:50 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_word(t_token **tokens, char *input, int *i)
{
	int		start;
	char	quote;
	char	*word;

	start = *i;
	while (input[*i] && !is_special(input[*i]) && !is_space(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[(*i)++];
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	word = ft_substr(input, start, *i - start);
	add_token(tokens, WORD, word);
	free(word);
}

static void	handle_pipe(t_token **tokens, int *i)
{
	add_token(tokens, PIPE, "|");
	(*i)++;
}

static void	handle_less(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		add_token(tokens, HEREDOC, "<<");
		*i += 2;
	}
	else
	{
		add_token(tokens, REDIR_IN, "<");
		(*i)++;
	}
}

static void	handle_greater(t_token **tokens, char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		add_token(tokens, APPEND, ">>");
		*i += 2;
	}
	else
	{
		add_token(tokens, REDIR_OUT, ">");
		(*i)++;
	}
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
		else
			read_word(&tokens, input, &i);
	}
	return (tokens);
}
