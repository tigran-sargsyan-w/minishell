/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:39:30 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 18:06:53 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"
# include <sys/types.h>

typedef struct s_pipe	t_pipe;

typedef struct s_pipe
{
	union
	{
		int				fds[2];
		struct
		{
			int			read;
			int			write;
		};
	};
}						t_pipe;

// executor_child.c
pid_t					fork_and_execute_cmd(t_cmd *current, t_shell *sh,
							int prev_fd, t_pipe pd);

// executor.c
void					executor(t_shell *sh);

// find_command.c
char					*find_command(char *cmd, char **envp);

// heredoc.c
int						handle_heredoc(t_redir *redir, t_shell *sh);

// redirection.c
int						handle_redirections(t_cmd *current_cmd, t_shell *sh);

// exit_utils.c
void					error_exit(char *msg);

// signals.c
void					setup_signal_handlers(void);
void					sigint_handler(int signo);

#endif