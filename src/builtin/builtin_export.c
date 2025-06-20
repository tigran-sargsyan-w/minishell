/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:15 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:38:56 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "ft_printf.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static int	concat_argument(char *key, char *value, t_env_list **env);
static int	export_or_concat(t_export_data *data, t_env_list **env);
static int	handle_export_arg(char *arg, t_env_list **env);

/**
 * @brief Concatenates a value to an existing environment variable.
 * If the variable does not exist, it creates a new one.
 * If the variable exists, it appends the value to the existing value.
 * @param key The key of the environment variable.
 * @param value The value to append.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	concat_argument(char *key, char *value, t_env_list **env)
{
	t_env_list	*node;
	char		*new_value;

	node = find_node_by_key(env, key);
	if (node == NULL)
		return (export_argument(key, value, env));
	if (node->value != NULL)
		new_value = ft_strjoin(node->value, value);
	else
		new_value = ft_strjoin("", value);
	if (new_value == NULL)
	{
		perror("malloc");
		return (1);
	}
	if (set_value(node, new_value) == NULL)
	{
		free(new_value);
		return (1);
	}
	free(new_value);
	return (0);
}

/**
 * @brief Exports the environment variable or concatenates it based on the type.
 * If the type is EXPORT, it exports the variable.
 * If the type is CONCAT, it concatenates the value to the existing variable.
 * @param data Pointer to the export data structure.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	export_or_concat(t_export_data *data, t_env_list **env)
{
	if (data->type == NONE)
		return (0);
	if (data->type == EXPORT)
		return (export_argument(data->key, data->value, env));
	return (concat_argument(data->key, data->value, env));
}

/**
 * @brief Handles the export argument, exporting or concatenating it.
 * If the argument is not a valid identifier, it prints an error message.
 * @param arg The argument to handle.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	handle_export_arg(char *arg, t_env_list **env)
{
	t_export_data	data;
	int				err;

	ft_memset(&data, 0, sizeof(data));
	if (!is_valid_export(arg, &data))
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	err = parse_key_value(arg, &data);
	if (err == 0)
		err = export_or_concat(&data, env);
	free_export_data(&data);
	return (err);
}

/**
 * @brief Exports the environment variables.
 * It prints all the environment variables in sorted order.
 * @param key The key of the environment variable.
 * @param value The value of the environment variable.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	export_argument(char *key, char *value, t_env_list **env)
{
	t_env_list	*node;

	node = find_node_by_key(env, key);
	if (node != NULL)
	{
		if (set_value(node, value) == NULL)
			return (1);
		return (0);
	}
	return (create_new_var(key, value, env));
}

/**
 * @brief Built-in command to export environment variables.
 * This function handles the 'export' command in the shell.
 * @param cmd The command structure containing the arguments.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	builtin_export(t_cmd *cmd, t_env_list **env)
{
	char	**argv;
	int		ret;

	ret = 0;
	if (cmd->args[1] == NULL)
		return (export_without_args(env));
	argv = &cmd->args[1];
	while (*argv != NULL)
	{
		if (handle_export_arg(*argv, env))
			ret = 1;
		argv++;
	}
	return (ret);
}
