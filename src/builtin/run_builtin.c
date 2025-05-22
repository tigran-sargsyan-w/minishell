/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:33 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/22 19:16:56 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stddef.h>
#include "parser.h"

int	run_builtin(t_cmd *cmd, t_shell *sh)
{
	int								i;
	static const t_builtins_array	builtins_arr[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"env", builtin_env},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"exit", builtin_exit},
	{NULL, NULL}};

	i = 0;
	if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
		return (-1);
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
