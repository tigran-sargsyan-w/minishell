/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:19:57 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 11:42:42 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static t_redir	*new_redir(t_redir_type type, const char *filename, int quoted);
static void		add_redir_node(t_redir **head, t_redir *node);
static void		add_redirection(t_cmd *cmd, t_redir_type type,
					const char *filename, int quoted);

/**
 * @brief Creates a new redirection node.
 * @param type The type of redirection (input, output, append, heredoc).
 * @param filename The name of the file for the redirection.
 * @param quoted Indicates if the filename is quoted.
 * @return Returns a pointer to the newly created redirection node, 
 * or NULL on failure.
 */
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

/**
 * @brief Adds a redirection node to the end of the list of redirections.
 * @param head Pointer to the head of the linked list.
 * @param node The redirection node to add.
 */
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

/**
 * @brief Adds a redirection to the command's redirection list.
 * @param cmd The command to which the redirection will be added.
 * @param type The type of redirection (input, output, append, heredoc).
 * @param filename The name of the file for the redirection.
 * @param quoted Indicates if the filename is quoted.
 */
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

/**
 * @brief Applies a redirection to the command based on the token type.
 * @param cmd The command to which the redirection will be applied.
 * @param redirect_type The type of redirection token 
 * (less, greater, dless, dgreater).
 * @param value The value associated with the redirection 
 * (filename or delimiter).
 * @param quoted Indicates if the value is quoted.
 */
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
