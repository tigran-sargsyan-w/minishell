/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:33 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/05 17:53:43 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "stddef.h"

int	run_builtin(t_cmd *cmd, t_env_list *env)
{
	int								i;
	static const t_builtins_array	builtins_arr[] = {{"echo", builtin_echo},
	{"cd", builtin_cd}, {"pwd", builtin_pwd}, {"env", builtin_env},
	{"export", builtin_export}, {"unset", builtin_unset}, {"exit",
		builtin_exit}, {NULL, NULL}};

	(void)env;
	i = 0;
	while (builtins_arr[i].builtin_name != NULL)
	{
		if (ft_strncmp(cmd->args[0], builtins_arr[i].builtin_name,
				(ft_strlen(cmd->args[0])) + 1) == 0)
		{
			builtins_arr[i].handler(cmd, env);
			break ;
		}
		i++;
	}
	return (-1);
}
