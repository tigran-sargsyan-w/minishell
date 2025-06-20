/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:39:27 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 11:23:42 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parser.h"
#include <stdlib.h>

static void	free_redirs(t_redir *redir);

/**
 * @brief Frees a linked list of redirection nodes.
 * @param redir Pointer to the head of the linked list of redirections.
 */
static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

/**
 * @brief Frees a linked list of command nodes.
 * Each command node contains arguments and redirections.
 * @param cmd Pointer to the head of the linked list of commands.
 */
void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->in_redirs)
			free_redirs(cmd->in_redirs);
		if (cmd->out_redirs)
			free_redirs(cmd->out_redirs);
		free(cmd);
		cmd = next;
	}
}

/**
 * @brief Frees an array of environment variables.
 * Each variable is a string, and the array is terminated by a NULL pointer.
 * @param env_tab Pointer to the array of environment variables.
 */
void	free_env_tab(char **env_tab)
{
	size_t	i;

	i = 0;
	if (env_tab == NULL)
		return ;
	while (env_tab[i])
	{
		free(env_tab[i]);
		i++;
	}
	free(env_tab);
}

/**
 * @brief Frees all environment variables in the shell structure.
 * It clears the linked list of environment variables and frees the array.
 * @param sh Pointer to the shell structure containing the environment list.
 */
void	free_all_env(t_shell *sh)
{
	lst_clear(&sh->env_list);
	free_env_tab(sh->env_tab);
}

/**
 * @brief Frees all resources used by the shell.
 * This includes freeing the command list, environment variables,
 * and closing all file descriptors.
 * @param sh Pointer to the shell structure containing all resources.
 */
void	free_all_resources(t_shell *sh)
{
	free_cmd_list(sh->cmd_list);
	free_all_env(sh);
	close_all_fds();
	sh->cmd_list = NULL;
	sh->env_list = NULL;
	sh->env_tab = NULL;
}
