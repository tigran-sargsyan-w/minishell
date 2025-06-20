/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:40:39 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 11:32:08 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdlib.h>

static int	expand_args(t_cmd *cmd);

/**
 * @brief Expands the command's argument array to accommodate more arguments.
 * @param cmd Pointer to the command structure.
 * @return Returns 0 on success, 1 on failure.
 * This function doubles the capacity of the command's argument array
 */
static int	expand_args(t_cmd *cmd)
{
	int		old_cap;
	int		new_cap;
	char	**new_args;
	int		i;

	old_cap = cmd->arg_cap;
	new_cap = old_cap * 2;
	new_args = malloc(sizeof(char *) * new_cap);
	if (!new_args)
		return (1);
	i = 0;
	while (i < old_cap)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	while (i < new_cap)
		new_args[i++] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_cap = new_cap;
	return (0);
}

/**
 * @brief Initializes a new command structure.
 * @return Returns a pointer to the initialized command structure, 
 * or NULL on failure.
 */
t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;
	int		i;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->arg_cap = INITIAL_ARG_CAP;
	cmd->args = malloc(sizeof(char *) * cmd->arg_cap);
	if (!cmd->args)
		return (free(cmd), NULL);
	i = 0;
	while (i < cmd->arg_cap)
		cmd->args[i++] = NULL;
	cmd->in_redirs = NULL;
	cmd->out_redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

/**
 * @brief Appends a new argument to the command's argument array.
 * @param cmd Pointer to the command structure.
 * @param value The argument value to append.
 * @return Returns 0 on success, 1 on failure.
 */
int	append_arg(t_cmd *cmd, char *value)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	if (i + 1 >= cmd->arg_cap)
	{
		if (expand_args(cmd))
			return (1);
	}
	cmd->args[i] = ft_strdup(value);
	if (!cmd->args[i])
		return (1);
	cmd->args[i + 1] = NULL;
	return (0);
}

/**
 * @brief Builds a single argument from a sequence of tokens.
 * @param tokens Pointer to the list of tokens.
 * @param sh Pointer to the shell structure.
 * @return Returns the combined argument string, or NULL on failure.
 */
char	*build_argument(t_token **tokens, t_shell *sh)
{
	t_token	*tok;
	char	*combined;
	char	*chunk;
	char	*new_combined;

	tok = *tokens;
	combined = ft_strdup("");
	if (!combined)
		return (NULL);
	while (tok && is_arg_token(tok->type) && (tok == *tokens
			|| tok->separated == 0))
	{
		chunk = get_chunk_for_token(tok, sh);
		if (!chunk)
			return (free(combined), NULL);
		new_combined = ft_strjoin(combined, chunk);
		free(combined);
		free(chunk);
		if (!new_combined)
			return (NULL);
		combined = new_combined;
		tok = tok->next;
	}
	*tokens = tok;
	return (combined);
}

/**
 * @brief Builds the delimiter for a heredoc from a sequence of tokens.
 * @param tokens Pointer to the list of tokens.
 * @param quoted Pointer to an integer indicating if the delimiter is quoted.
 * @param sh Pointer to the shell structure.
 * @return Returns the delimiter string, or NULL on failure.
 */
char	*build_heredoc_delim(t_token **tokens, int *quoted, t_shell *sh)
{
	t_token	*tok;
	char	*result;

	tok = *tokens;
	if (tok->type == TOK_SQUOTED || tok->type == TOK_DQUOTED)
		*quoted = 1;
	else
		*quoted = 0;
	if (*quoted)
		result = ft_strdup(tok->value);
	else
		result = expand_vars(tok->value, sh);
	*tokens = tok->next;
	return (result);
}
