/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:25:20 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 20:49:23 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new;
	t_token	*temp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->type = type;
	new->value = ft_strdup(value);
	new->next = NULL;
	if (!*tokens)
		*tokens = new;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == WORD)
			printf("WORD: [%s]\n", tokens->value);
		else if (tokens->type == PIPE)
			printf("PIPE: [%s]\n", tokens->value);
		else if (tokens->type == REDIR_IN)
			printf("REDIR_IN: [%s]\n", tokens->value);
		else if (tokens->type == REDIR_OUT)
			printf("REDIR_OUT: [%s]\n", tokens->value);
		else if (tokens->type == HEREDOC)
			printf("HEREDOC: [%s]\n", tokens->value);
		else if (tokens->type == APPEND)
			printf("APPEND: [%s]\n", tokens->value);
		tokens = tokens->next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
