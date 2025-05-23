/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:58:43 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/23 17:52:13 by tsargsya         ###   ########.fr       */
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
		{
			t_redir *redir = cmd->in_redirs;
			while (redir)
			{
				t_redir *temp = redir;
				redir = redir->next;
				free(temp->filename);
				free(temp);
			}
		}
		if (cmd->out_redirs)
		{
			t_redir *redir = cmd->out_redirs;
			while (redir)
			{
				t_redir *temp = redir;
				redir = redir->next;
				free(temp->filename);
				free(temp);
			}
		}
		free(cmd);
		cmd = next;
	}
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*current_cmd;
	t_cmd	*new_cmd;
	int		redirect_type;

	cmd = init_cmd();
	current_cmd = cmd;
	while (tokens)
	{
		if (tokens->type == TOK_WORD)
			append_arg(current_cmd, tokens->value);
		else if (tokens->type == TOK_LESS || tokens->type == TOK_GREATER
			|| tokens->type == TOK_DLESS || tokens->type == TOK_DGREATER)
		{
			if (!tokens->next || tokens->next->type != TOK_WORD)
			{
				printf("minishell: syntax error near `%s'\n", tokens->value);
				free_cmd_list(cmd);
				return (NULL);
			}
			redirect_type = tokens->type;
			tokens = tokens->next;
			if (redirect_type == TOK_LESS)
			{
				add_redirection(current_cmd, REDIR_IN, tokens->value);
			}
			else if (redirect_type == TOK_GREATER)
			{
				add_redirection(current_cmd, REDIR_OUT, tokens->value);
			}
			else if (redirect_type == TOK_DLESS)
			{
				add_redirection(current_cmd, REDIR_HEREDOC, tokens->value);
			}
			else if (redirect_type == TOK_DGREATER)
			{
				add_redirection(current_cmd, REDIR_APPEND, tokens->value);
			}
		}
		else if (tokens->type == TOK_PIPE)
		{
			if (!current_cmd || (current_cmd->args[0] == NULL))
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				free_cmd_list(cmd);
				return (NULL);
			}
			if (!tokens->next || tokens->next->type != TOK_WORD)
			{
				printf("minishell: syntax error near unexpected token ");
				if (!tokens->next)
					printf("`newline'\n");
				else
					printf("`%s'\n", tokens->next->value);
				free_cmd_list(cmd);
				return (NULL);
			}
			new_cmd = init_cmd();
			if (!new_cmd)
			{
				free_cmd_list(cmd);
				return (NULL);
			}
			current_cmd->next = new_cmd;
			current_cmd = new_cmd;
		}
		else
		{
			printf("minishell: syntax error near unexpected token `%s'\n",
				tokens->value);
			free_cmd_list(cmd);
			return (NULL);
		}
		tokens = tokens->next;
	}
	return (cmd);
}

void	print_cmds(t_cmd *cmd)
{
	int	i;
	int	cmd_num;

	cmd_num = 1;
	while (cmd)
	{
		printf("Command %d:\n", cmd_num++);
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("  arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		if (cmd->in_redirs)
		{
			t_redir *redir = cmd->in_redirs;
			while (redir)
			{
				printf("  in_redir: %s\n", redir->filename);
				redir = redir->next;
			}
		}
		if (cmd->out_redirs)
		{
			t_redir *redir = cmd->out_redirs;
			while (redir)
			{
				printf("  out_redir: %s\n", redir->filename);
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
}
