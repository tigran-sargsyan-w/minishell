/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:42:57 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 23:04:49 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"

/**
 * @brief Checks if the token type is an argument token.
 * @param type The token type to check.
 * @return Returns 1 if the token type is an argument token, 0 otherwise.
 * Argument tokens are those that represent command arguments,
 * such as words, single-quoted strings, and double-quoted strings.
 */
int	is_arg_token(t_token_type type)
{
	return (type == TOK_WORD || type == TOK_SQUOTED || type == TOK_DQUOTED);
}

/**
 * @brief Checks if the token type is a redirection token.
 * @param type The token type to check.
 * @return Returns 1 if the token type is a redirection token, 0 otherwise.
 * Redirection tokens include input/output redirections and here-documents.
 */
int	is_redir_token(t_token_type type)
{
	return (type == TOK_LESS || type == TOK_GREATER || type == TOK_DLESS
		|| type == TOK_DGREATER);
}

/**
 * @brief Checks if the command is valid before a pipe.
 * A command is considered valid before a pipe if it has arguments
 * or input/output redirections.
 * @param cmd Pointer to the command structure to check.
 * @return Returns 1 if the command is valid, 0 otherwise.
 */
int	cmd_is_valid_before_pipe(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->args && cmd->args[0])
		return (1);
	if (cmd->in_redirs || cmd->out_redirs)
		return (1);
	return (0);
}

/**
 * @brief Gets the chunk of text for a given token.
 * @param tok The token for which to get the chunk.
 * @param sh The shell structure.
 * @return Returns a newly allocated string containing the chunk of text
 * for the token, or NULL on failure.
 */
char	*get_chunk_for_token(t_token *tok, t_shell *sh)
{
	char	*chunk;

	chunk = NULL;
	if (tok->type == TOK_SQUOTED)
		chunk = ft_strdup(tok->value);
	else if (tok->type == TOK_WORD || tok->type == TOK_DQUOTED)
		chunk = expand_vars(tok->value, sh);
	if (!chunk && (tok->type == TOK_WORD || tok->type == TOK_DQUOTED))
		chunk = ft_strdup("");
	return (chunk);
}
