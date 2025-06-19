/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:27 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 19:33:06 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * @brief Process a line read from the heredoc input.
 * This function checks if the line matches the
 * heredoc delimiter and writes the line to the file descriptor.
 * @param line The line read from the input.
 * @param fd The file descriptor to write to.
 * @param redir The heredoc redirection structure containing the delimiter.
 * @param sh The shell structure containing environment variables.
 * @return Returns 0 if the line matches the delimiter,
 *         otherwise returns 1.
 */
static int	process_heredoc_line(char *line, int fd, t_redir *redir,
		t_shell *sh)
{
	char	*expanded;

	if (ft_strcmp(line, redir->filename) == 0)
	{
		free(line);
		return (0);
	}
	if (redir->quoted == 0)
	{
		expanded = expand_vars(line, sh);
		if (expanded != NULL)
			write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
	}
	write(fd, "\n", 1);
	free(line);
	return (1);
}

/**
 * @brief Writes the heredoc content to a temporary file.
 * @param redir The heredoc redirection structure.
 * @param sh The shell structure containing environment variables.
 * @return Returns 0 on success, or -1 on failure.
 */
static int	write_heredoc_content(t_redir *redir, t_shell *sh)
{
	int		fd;
	char	*line;
	int		tty;

	tty = open("/dev/tty", O_RDONLY);
	if (tty < 0)
		error_exit("open /dev/tty");
	if (dup2(tty, STDIN_FILENO) < 0)
		error_exit("dup2 tty");
	close(tty);
	fd = open(HEREDOC_TMPFILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error_exit("open heredoc");
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!process_heredoc_line(line, fd, redir, sh))
			break ;
	}
	close(fd);
	free_all_env(sh);
	free_cmd_list(sh->cmd_list);
	exit(0);
}

/**
 * @brief Waits for the heredoc process to finish and handles signals.
 * @param pid The process ID of the heredoc child process.
 * @param sh The shell structure containing the last status.
 * @return Returns 0 on success, or -1 if interrupted by SIGINT.
 */
static int	wait_for_heredoc(pid_t pid, t_shell *sh)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(HEREDOC_TMPFILE);
		free_all_resources(sh);
		sh->last_status = 130;
		return (-1);
	}
	return (0);
}

/**
 * @brief Handles the heredoc redirection.
 * This function forks a child process to read input for the heredoc,
 * writes it to a temporary file, and updates the redirection structure.
 * @param redir The heredoc redirection structure.
 * @param sh The shell structure containing environment variables.
 * @return Returns 0 on success, or -1 on failure.
 */
int	handle_heredoc(t_redir *redir, t_shell *sh)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		write_heredoc_content(redir, sh);
	}
	else if (pid > 0)
	{
		if (wait_for_heredoc(pid, sh) < 0)
			return (-1);
	}
	else
	{
		perror("fork");
		return (-1);
	}
	free(redir->filename);
	redir->filename = ft_strdup(HEREDOC_TMPFILE);
	if (!redir->filename)
		return (-1);
	redir->type = REDIR_IN;
	return (0);
}
