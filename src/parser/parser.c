/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:43 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 21:52:57 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "parser.h"
// #include "minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int	expand_args(t_cmd *cmd)
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

void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->in_redirs)
			free_redirs(cmd->in_redirs);
		if (cmd->out_redirs)
			free_redirs(cmd->out_redirs);
		free(cmd);
		cmd = next;
	}
}

int	is_arg_token(t_token_type type)
{
	return (type == TOK_WORD || type == TOK_SQUOTED || type == TOK_DQUOTED);
}

int	is_redir_token(t_token_type type)
{
	return (type == TOK_LESS || type == TOK_GREATER || type == TOK_DLESS
		|| type == TOK_DGREATER);
}

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

static char	*get_chunk_for_token(t_token *tok, t_shell *sh)
{
	char	*chunk;

	chunk = NULL;
	if (tok->type == TOK_SQUOTED)
		chunk = ft_strdup(tok->value);
	else if (tok->type == TOK_WORD || tok->type == TOK_DQUOTED)
		chunk = expand_vars(tok->value, sh);
	if (!chunk && (tok->type == TOK_SQUOTED || tok->type == TOK_DQUOTED))
		chunk = ft_strdup("");
	return (chunk);
}

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

// Argument handling
static int handle_argument_token(t_token **tokens, t_cmd *cmd, t_shell *sh)
{
    char *value = build_argument(tokens, sh);
    if (!value)
        return FAILURE;
    if (append_arg(cmd, value))
        return (free(value), FAILURE);
    free(value);
    return SUCCESS;
}

// Redirection handling
static int handle_redirection_token(t_token **tokens, t_cmd *cmd, t_shell *sh)
{
    int  quoted = 0;
    int  redirect_type = (*tokens)->type;
    char *value;

    if (!(*tokens)->next || !is_arg_token((*tokens)->next->type))
    {
        ft_dprintf(2, "minishell: syntax error near `%s'\n", (*tokens)->value);
        sh->last_status = 2;
        return FAILURE;
    }
    *tokens = (*tokens)->next;

    if (redirect_type == TOK_DLESS)
        value = build_heredoc_delim(tokens, &quoted, sh);
    else
        value = build_argument(tokens, sh);
    if (!value)
        return FAILURE;

    if (redirect_type == TOK_LESS)
        add_redirection(cmd, REDIR_IN, value, 0);
    else if (redirect_type == TOK_GREATER)
        add_redirection(cmd, REDIR_OUT, value, 0);
    else if (redirect_type == TOK_DLESS)
        add_redirection(cmd, REDIR_HEREDOC, value, quoted);
    else if (redirect_type == TOK_DGREATER)
        add_redirection(cmd, REDIR_APPEND, value, 0);

    free(value);
    return SUCCESS;
}

// Pipe handling
static int handle_pipe_token(t_token **tokens, t_cmd **current_cmd, t_shell *sh)
{
    t_cmd *new_cmd;

    if (!cmd_is_valid_before_pipe(*current_cmd))
    {
        sh->last_status = 2;
        ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
        return FAILURE;
    }
    if (!(*tokens)->next)
    {
        sh->last_status = 2;
        ft_dprintf(2, "minishell: syntax error near unexpected token `newline'\n");
        return FAILURE;
    }
    new_cmd = init_cmd();
    if (!new_cmd)
        return FAILURE;
    (*current_cmd)->next = new_cmd;
    *current_cmd = new_cmd;
    *tokens = (*tokens)->next;
    return SUCCESS;
}

// Main function
t_cmd *parse_tokens(t_token *tokens, t_shell *sh)
{
    t_cmd *cmd = init_cmd();
    t_cmd *current_cmd = cmd;

    if (!cmd)
        return NULL;

    while (tokens)
    {
        if (is_arg_token(tokens->type))
        {
            if (handle_argument_token(&tokens, current_cmd, sh) == FAILURE)
                return (free_cmd_list(cmd), NULL);
            continue;
        }
        else if (is_redir_token(tokens->type))
        {
            if (handle_redirection_token(&tokens, current_cmd, sh) == FAILURE)
                return (free_cmd_list(cmd), NULL);
            continue;
        }
        else if (tokens->type == TOK_PIPE)
        {
            if (handle_pipe_token(&tokens, &current_cmd, sh) == FAILURE)
                return (free_cmd_list(cmd), NULL);
            continue;
        }
        else
        {
            sh->last_status = 2;
            ft_dprintf(2,
                "minishell: syntax error near unexpected token `%s'\n",
                tokens->value);
            return (free_cmd_list(cmd), NULL);
        }
    }
    return cmd;
}
