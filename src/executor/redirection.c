/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 14:56:23 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <unistd.h>

static int	open_redirection_file(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		perror(redir->filename);
	return (fd);
}

// Duplicates fd to stdin or stdout, closes it, and returns 0 or -1
static int	redirect_fd_to_stdio(int fd, t_redir *redir)
{
	int	ret;

	if (redir->type == REDIR_IN)
		ret = dup2(fd, STDIN_FILENO);
	else
		ret = dup2(fd, STDOUT_FILENO);
	if (ret < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// Applies a single redirection (including heredoc)
static int	apply_one_redir(t_redir *redir, t_shell *sh)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
		return (handle_heredoc(redir, sh));
	fd = open_redirection_file(redir);
	if (fd < 0)
		return (-1);
	return (redirect_fd_to_stdio(fd, redir));
}

int	handle_redirections(t_cmd *cmd, t_shell *sh)
{
	t_redir	*redir;

	redir = cmd->in_redirs;
	while (redir)
	{
		if (apply_one_redir(redir, sh) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = cmd->out_redirs;
	while (redir)
	{
		if (apply_one_redir(redir, sh) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
