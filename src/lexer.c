/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:33:07 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 23:39:09 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_quotes(char *input, char *buffer, int *i, int *j)
{
	char	quote;

	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
	{
		buffer[(*j)++] = input[(*i)++];
	}
	if (input[*i] == quote)
		(*i)++;
}

void	read_word(t_token **tokens, char *input, int *i)
{
	char	*word;
	char	*buffer;
	int		j;

	j = 0;
	buffer = malloc(sizeof(char) * (strlen(input) + 1));
	if (!buffer)
		return ;
	j = 0;
	while (input[*i] && !is_special(input[*i]) && !is_space(input[*i]))
	{
		if (input[*i] == '\\' && input[*i + 1])
		{
			(*i)++;
			buffer[j++] = input[*i];
			(*i)++;
		}
		else if (input[*i] == '\'' || input[*i] == '"')
			skip_quotes(input, buffer, i, &j);
		else
			buffer[j++] = input[(*i)++];
	}
	buffer[j] = '\0';
	word = ft_strdup(buffer);
	add_token(tokens, WORD, word);
	free(word);
	free(buffer);
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
