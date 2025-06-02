/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:25:20 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/31 20:22:37 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	add_token(t_token **tokens, t_token_type type, char *value,
		int separated)
{
	t_token	*new;
	t_token	*temp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->type = type;
	new->value = ft_strdup(value);
	new->separated = separated;
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
	printf(YELLOW "TOKENS:\n" RESET);
	while (tokens)
	{
		switch (tokens->type)
		{
		case TOK_WORD:
			printf("WORD      : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value); // <<=== печатаем sep для отладки
			break ;
		case TOK_SQUOTED:
			printf("SQUOTED   : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value);
			break ;
		case TOK_DQUOTED:
			printf("DQUOTED   : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value);
			break ;
		case TOK_PIPE:
			printf("PIPE      : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value ? tokens->value : "|");
			break ;
		case TOK_LESS:
			printf("REDIR_IN  : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value ? tokens->value : "<");
			break ;
		case TOK_GREATER:
			printf("REDIR_OUT : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value ? tokens->value : ">");
			break ;
		case TOK_DLESS:
			printf("HEREDOC   : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value ? tokens->value : "<<");
			break ;
		case TOK_DGREATER:
			printf("APPEND    : sep=%d  [%s]\n",
					tokens->separated,
					tokens->value ? tokens->value : ">>");
			break ;
		default:
			printf("UNKNOWN(%d): sep=%d  [%s]\n",
					tokens->type,
					tokens->separated,
					tokens->value ? tokens->value : "");
			break ;
		}
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
