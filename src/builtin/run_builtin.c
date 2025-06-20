/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:33 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:45:55 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "parser.h"
#include <stdlib.h>

static int	handle_exit(t_shell *sh, t_cmd *cmd);

/**
 * @brief Handles the exit built-in command.
 * This function is called when the user wants to exit the shell.
 * It calls the built-in exit function and frees all resources before
 * terminating the shell.
 * @param sh Pointer to the shell structure.
 * @param cmd Pointer to the command structure.
 * @return The exit status of the shell.
 */
static int	handle_exit(t_shell *sh, t_cmd *cmd)
{
	if (builtin_exit(sh, cmd->args) == 1)
	{
		free_all_resources(sh);
		exit(sh->last_status);
	}
	else
		return (sh->last_status);
}

/**
 * @brief Runs a built-in command.
 * This function checks the command against a list of built-in commands
 * and executes the corresponding handler if a match is found.
 * @param cmd Pointer to the command structure.
 * @param sh Pointer to the shell structure.
 * @return The exit status of the built-in command, or -1 if not found.
 */
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
		return (handle_exit(sh, cmd));
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
