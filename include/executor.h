/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:39:30 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/06 18:23:35 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"
# include <sys/types.h>
# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127
# define HEREDOC_TMPFILE "minishell_heredoc"

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

// find_command.c
char					*find_command(char *cmd, char **envp);
void					free_array(char **array);

// executor.c
void					executor(t_cmd *cmd, t_shell *sh);
void					execute_cmds(t_cmd *cmd, t_shell *sh);

// executor_utils.c
pid_t					fork_and_execute_cmd(t_cmd *cmd, t_shell *sh,
							int prev_fd, t_pipe pd);
void					execute_child(t_cmd *cmd, t_shell *sh);
int						handle_redirections(t_cmd *cmd, t_shell *sh);

// exit_utils.c
void					error_exit(char *msg);

// signals.c
void					setup_signal_handlers(void);

#endif