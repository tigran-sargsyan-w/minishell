/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:39:30 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/17 10:02:30 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"
# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127

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
void					execute_cmds(t_cmd *cmd, char **envp,
							t_shell *sh);

// executor_utils.c
void					fork_and_execute_cmd(t_cmd *cmd, char **envp,
							int prev_fd, t_pipe pd, t_env_list **env_variables);
void					execute_child(t_cmd *cmd, char **envp,
							t_env_list **env_variables);
void					handle_input_redirection(t_cmd *cmd);
void					handle_output_redirection(t_cmd *cmd);

// exit_utils.c
void					error_exit(char *msg);

#endif