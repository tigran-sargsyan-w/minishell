/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:18:09 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/10 20:18:10 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void	clean_shell(t_shell *sh)
{
	if (!sh)
		return ;
	if (sh->env_tab)
		free_env_tab(sh->env_tab);
	if (sh->env_list)
		lst_clear(&sh->env_list);
}
