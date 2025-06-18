/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:39:27 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/18 15:18:29 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parser.h"
#include <stdlib.h>

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

void	free_all_env(t_shell *sh)
{
	lst_clear(&sh->env_list);
	free_env_tab(sh->env_tab);
}

void	free_all_resources(t_shell *sh)
{
	free_cmd_list(sh->cmd_list);
	free_all_env(sh);
	close_all_fds();
	sh->cmd_list = NULL;
	sh->env_list = NULL;
	sh->env_tab = NULL;
}
