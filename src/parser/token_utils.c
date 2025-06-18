/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:42:57 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/18 16:57:00 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"

int	is_arg_token(t_token_type type)
{
	return (type == TOK_WORD || type == TOK_SQUOTED || type == TOK_DQUOTED);
}

int	is_redir_token(t_token_type type)
{
	return (type == TOK_LESS || type == TOK_GREATER || type == TOK_DLESS
		|| type == TOK_DGREATER);
}

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
