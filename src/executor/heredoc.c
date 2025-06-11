/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:27:27 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/11 14:54:17 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

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

// Main function for writing heredoc
static int	write_heredoc_content(t_redir *redir, t_shell *sh)
{
	int		fd;
	char	*line;

	fd = open(HEREDOC_TMPFILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open heredoc");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!process_heredoc_line(line, fd, redir, sh))
			break ;
	}
	close(fd);
	exit(0);
}

static int	wait_for_heredoc(pid_t pid, t_shell *sh)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(HEREDOC_TMPFILE);
		sh->last_status = 130;
		return (-1);
	}
	return (0);
}

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
