/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:06 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/02 20:09:42 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "ft_printf.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>


//TODO: Handle exit status code overflow
int	builtin_exit(t_cmd *cmd, t_env_list **env)
{
	lst_clear(env);
	if (cmd->args[1] == NULL)
		exit(0);
	else
	{
		if (cmd->args[2])
			ft_dprintf(2, "exit\nminishell: exit: too many arguments\n");
		exit(ft_atoi(cmd->args[1]));
	}
	return (0);
}
