/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:40 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 19:35:08 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <unistd.h>

/**
 * @brief Opens the redirection file based on the type of redirection.
 * @param redir The redirection structure containing the type and filename.
 * @return Returns the file descriptor on success, or -1 on failure.
 */
static int	open_redirection_file(t_redir *redir)
{
	int	fd;

	fd = -1;
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

/**
 * @brief Redirects the file descriptor to standard input or output.
 * @param fd The file descriptor to redirect.
 * @param redir The redirection structure containing the type of redirection.
 * @return Returns 0 on success, or -1 on failure.
 */
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

/**
 * @brief Applies a single redirection by opening the file and redirecting
 * the file descriptor to standard input or output.
 * @param redir The redirection structure containing the type and filename.
 * @return Returns 0 on success, or -1 on failure.
 */
static int	apply_one_redir(t_redir *redir)
{
	int	fd;

	fd = open_redirection_file(redir);
	if (fd < 0)
		return (-1);
	return (redirect_fd_to_stdio(fd, redir));
}

/**
 * @brief Handles all redirections for a command.
 * This function applies all input and output redirections
 * specified in the command structure.
 * @param current_cmd The command structure containing redirections.
 * @return Returns 0 on success, or -1 on failure.
 */
int	handle_redirections(t_cmd *current_cmd)
{
	t_redir	*redir;

	redir = current_cmd->in_redirs;
	while (redir)
	{
		if (apply_one_redir(redir) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = current_cmd->out_redirs;
	while (redir)
	{
		if (apply_one_redir(redir) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
