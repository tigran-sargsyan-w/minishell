/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:19:57 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 23:58:44 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static t_redir	*new_redir(t_redir_type type, const char *filename, int quoted)
{
	t_redir	*redir;

	redir = malloc(sizeof(*redir));
	if (!redir)
	{
		perror("malloc");
		return (NULL);
	}
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		perror("strdup");
		free(redir);
		return (NULL);
	}
	redir->quoted = quoted;
	redir->next = NULL;
	return (redir);
}

static void	add_redir_node(t_redir **head, t_redir *node)
{
	t_redir	*cur;

	if (!*head)
		*head = node;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
}

static void	add_redirection(t_cmd *cmd, t_redir_type type, const char *filename,
		int quoted)
{
	t_redir	*redir;

	redir = new_redir(type, filename, quoted);
	if (!redir)
		return ;
	if (type == REDIR_IN || type == REDIR_HEREDOC)
		add_redir_node(&cmd->in_redirs, redir);
	else if (type == REDIR_OUT || type == REDIR_APPEND)
		add_redir_node(&cmd->out_redirs, redir);
}

void	apply_redirection(t_cmd *cmd, int redirect_type, const char *value,
		int quoted)
{
	if (redirect_type == TOK_LESS)
		add_redirection(cmd, REDIR_IN, value, 0);
	else if (redirect_type == TOK_GREATER)
		add_redirection(cmd, REDIR_OUT, value, 0);
	else if (redirect_type == TOK_DLESS)
		add_redirection(cmd, REDIR_HEREDOC, value, quoted);
	else if (redirect_type == TOK_DGREATER)
		add_redirection(cmd, REDIR_APPEND, value, 0);
}
