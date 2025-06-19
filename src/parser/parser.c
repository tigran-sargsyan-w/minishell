/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:43 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 23:00:32 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "parser.h"
#include <stdlib.h>

/**
 * @brief Handles an argument token by building the argument string 
 * and appending it to the command.
 * @param tokens Pointer to the list of tokens.
 * @param cmd Pointer to the command structure.
 * @param sh Pointer to the shell structure.
 * @return Returns 0 on success, 1 on failure.
 */
static int	handle_argument_token(t_token **tokens, t_cmd *cmd, t_shell *sh)
{
	char	*value;

	value = build_argument(tokens, sh);
	if (!value)
		return (FAILURE);
	if (append_arg(cmd, value))
		return (free(value), FAILURE);
	free(value);
	return (SUCCESS);
}

/**
 * @brief Handles a redirection token by building the redirection value 
 * and applying it to the command.
 * @param tokens Pointer to the list of tokens.
 * @param cmd Pointer to the command structure.
 * @param sh Pointer to the shell structure.
 * @return Returns 0 on success, 1 on failure.
 */
static int	handle_redirection_token(t_token **tokens, t_cmd *cmd, t_shell *sh)
{
	int		quoted;
	int		redirect_type;
	char	*value;

	quoted = 0;
	redirect_type = (*tokens)->type;
	if (!(*tokens)->next || !is_arg_token((*tokens)->next->type))
	{
		ft_dprintf(2, "minishell: syntax error near `%s'\n", (*tokens)->value);
		sh->last_status = 2;
		return (FAILURE);
	}
	*tokens = (*tokens)->next;
	if (redirect_type == TOK_DLESS)
		value = build_heredoc_delim(tokens, &quoted, sh);
	else
		value = build_argument(tokens, sh);
	if (!value)
		return (FAILURE);
	apply_redirection(cmd, redirect_type, value, quoted);
	free(value);
	return (SUCCESS);
}

/**
 * @brief Handles a pipe token by validating the command before the pipe 
 * and initializing a new command for the next part of the pipeline.
 * @param tokens Pointer to the list of tokens.
 * @param current_cmd Pointer to the current command structure.
 * @param sh Pointer to the shell structure.
 * @return Returns 0 on success, 1 on failure.
 */
static int	handle_pipe_token(t_token **tokens, t_cmd **current_cmd,
		t_shell *sh)
{
	t_cmd	*new_cmd;

	if (!cmd_is_valid_before_pipe(*current_cmd))
	{
		sh->last_status = 2;
		ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
		return (FAILURE);
	}
	if (!(*tokens)->next)
	{
		sh->last_status = 2;
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		return (FAILURE);
	}
	new_cmd = init_cmd();
	if (!new_cmd)
		return (FAILURE);
	(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	*tokens = (*tokens)->next;
	return (SUCCESS);
}

/**
 * @brief Processes a token based on its type and handles it accordingly.
 * @param tokens Pointer to the list of tokens.
 * @param current_cmd Pointer to the current command structure.
 * @param sh Pointer to the shell structure.
 * @return Returns 0 on success, 1 on failure.
 */
static int	process_token(t_token **tokens, t_cmd **current_cmd, t_shell *sh)
{
	if (is_arg_token((*tokens)->type))
		return (handle_argument_token(tokens, *current_cmd, sh));
	if (is_redir_token((*tokens)->type))
		return (handle_redirection_token(tokens, *current_cmd, sh));
	if ((*tokens)->type == TOK_PIPE)
		return (handle_pipe_token(tokens, current_cmd, sh));
	sh->last_status = 2;
	ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
		(*tokens)->value);
	return (FAILURE);
}

/**
 * @brief Parses a list of tokens and constructs a command list.
 * @param tokens Pointer to the list of tokens to parse.
 * @param sh Pointer to the shell structure.
 * @return Returns a pointer to the command list on success, NULL on failure.
 */
t_cmd	*parse_tokens(t_token *tokens, t_shell *sh)
{
	t_cmd	*cmd;
	t_cmd	*current_cmd;

	cmd = init_cmd();
	current_cmd = cmd;
	if (!cmd)
		return (NULL);
	while (tokens)
	{
		if (process_token(&tokens, &current_cmd, sh) == FAILURE)
		{
			sh->last_status = 2;
			free_cmd_list(cmd);
			return (NULL);
		}
	}
	return (cmd);
}
