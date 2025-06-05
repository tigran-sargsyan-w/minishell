/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:33 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/05 05:11:46 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "ft_printf.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdlib.h>

int	builtin_exit(t_shell *sh, t_cmd *cmd)
{
	if (cmd->args[1] == NULL)
	{
		ft_dprintf(1, "exit\n");
		return (sh->last_status);
	}
	else
	{
		if (cmd->args[2])
			ft_dprintf(2, "exit\nminishell: exit: too many arguments\n");
		ft_dprintf(2, "exit\n");
		return (ft_atoi(cmd->args[1]) % 256);
	}
}

int	run_builtin(t_cmd *cmd, t_shell *sh)
{
	int								i;
	static const t_builtins_array	builtins_arr[] = {{"echo", builtin_echo},
			{"cd", builtin_cd}, {"pwd", builtin_pwd}, {"env", builtin_env},
			{"export", builtin_export}, {"unset", builtin_unset}, {NULL, NULL}};

	i = 0;
	if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
		return (-1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		lst_clear(&sh->env_list);
		free_env_tab(sh->env_tab);
		exit(builtin_exit(sh, cmd));
	}
	while (builtins_arr[i].builtin_name != NULL)
	{
		if (ft_strcmp(cmd->args[0], builtins_arr[i].builtin_name) == 0)
		{
			sh->last_status = builtins_arr[i].handler(cmd, &sh->env_list);
			return (sh->last_status);
		}
		i++;
	}
	return (-1);
}
