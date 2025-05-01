/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:43 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/01 16:44:38 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
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

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*current_cmd;
	int		i;
	int		redirect_type;

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
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	current_cmd = cmd;
	while (tokens)
	{
		if (tokens->type == WORD)
			append_arg(current_cmd, tokens->value);
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == HEREDOC || tokens->type == APPEND)
		{
			if (!tokens->next || tokens->next->type != WORD)
			{
				printf("minishell: syntax error near `%s'\n", tokens->value);
				// TODO: free_cmd_list(cmd);
				return (NULL);
			}
			redirect_type = tokens->type;
			tokens = tokens->next;
			if (redirect_type == REDIR_IN)
				current_cmd->infile = ft_strdup(tokens->value);
			else if (redirect_type == REDIR_OUT)
				current_cmd->outfile = ft_strdup(tokens->value);
			else if (redirect_type == HEREDOC)
			{
				current_cmd->heredoc = 1;
				current_cmd->infile = ft_strdup(tokens->value);
			}
			else if (redirect_type == APPEND)
			{
				current_cmd->append = 1;
				current_cmd->outfile = ft_strdup(tokens->value);
			}
		}
		else if (tokens->type == PIPE)
		{
			// TODO: logic for creating a new command
			// TODO: handle pipe
		}
		else
		{
			printf("minishell: syntax error near unexpected token `%s'\n",
				tokens->value);
			// TODO: free_cmd_list(cmd);
			return (NULL);
		}
		tokens = tokens->next;
	}
	return (cmd);
}
