/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:39:30 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/09 15:11:02 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "parser.h"

typedef struct s_pipe
{
	int	read;
	int	write;
}		t_pipe;

//executor_utils.c
char	*find_command(char *cmd, char **envp);
void	free_array(char **array);

//executor.c
void	execute_cmd(t_cmd *cmd, char **envp);
void	execute_pipeline(t_cmd *cmd, char **envp, t_env_list **env_vars);

#endif