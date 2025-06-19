/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:25:20 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:44:31 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdlib.h>

/**
 * @brief Checks if a character is a whitespace character.
 * @param c The character to check.
 * @return 1 if the character is a whitespace, 0 otherwise.
 */
int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

/**
 * @brief Checks if a character is a special character.
 * @param c The character to check.
 * @return 1 if the character is a special character, 0 otherwise.
 */
int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * @brief Adds a new token to the end of the token list.
 * @param tokens Pointer to the pointer of the first token in the list.
 * @param type The type of the token.
 * @param value The value of the token.
 * @param separated Indicates if the token is separated by whitespace.
 */
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

/**
 * @brief Frees the memory allocated for the token list.
 * @param tokens Pointer to the first token in the list.
 */
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
