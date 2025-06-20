/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:31:44 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static int	update_var(t_env_list **env, char *key, char *value);
static int	get_target_path(t_cmd *cmd, t_env_list **env, char **dst);
static char	*safe_getcwd(t_env_list **env);
static int	perform_cd(char *target, char **old_pwd, char **new_pwd,
				t_env_list **env);

/**
 * @brief Updates the environment variable with the given key and value.
 * If the variable already exists, it updates its value.
 * If it does not exist, it creates a new variable.
 * @param env Pointer to the environment list.
 * @param key The key of the environment variable.
 * @param value The value to set for the environment variable.
 * @return 0 on success, 1 on failure.
 */
static int	update_var(t_env_list **env, char *key, char *value)
{
	t_env_list	*node;
	char		*temp_key;
	char		*temp_value;
	int			ret;

	node = find_node_by_key(env, key);
	if (node)
	{
		if (!set_value(node, value))
			return (1);
		return (0);
	}
	if (safe_strdup_pair(key, value, &temp_key, &temp_value))
		return (1);
	ret = export_argument(temp_key, temp_value, env);
	free(temp_key);
	free(temp_value);
	return (ret);
}

/**
 * @brief Gets the target path for the cd command.
 * @param cmd The command structure.
 * @param env Pointer to the environment list.
 * @param dst Pointer to the destination string.
 * @return 0 on success, 1 on failure.
 */
static int	get_target_path(t_cmd *cmd, t_env_list **env, char **dst)
{
	t_env_list	*home;

	if (cmd->args[1] == NULL)
	{
		home = find_node_by_key(env, "HOME");
		if (!home || !home->value)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		*dst = ft_strdup(home->value);
	}
	else if (cmd->args[2] != NULL)
	{
		ft_putendl_fd("minishell: cd: Too many arguments", 2);
		return (1);
	}
	else
		*dst = ft_strdup(cmd->args[1]);
	if (!*dst)
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

/**
 * @brief Safely gets the current working directory.
 * If getcwd fails, it tries to retrieve the PWD from the environment.
 * @param env Pointer to the environment list.
 * @return The current working directory or NULL on failure.
 */
static char	*safe_getcwd(t_env_list **env)
{
	char		*cwd;
	t_env_list	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd = find_node_by_key(env, "PWD");
		if (pwd && pwd->value)
			cwd = ft_strdup(pwd->value);
	}
	return (cwd);
}

/**
 * @brief Performs the actual change of directory.
 * Changes the current working directory to the target path.
 * If successful, updates the PWD and OLDPWD environment variables.
 * @param target The target directory to change to.
 * @param old_pwd Pointer to store the old working directory.
 * @param new_pwd Pointer to store the new working directory.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
static int	perform_cd(char *target, char **old_pwd, char **new_pwd,
		t_env_list **env)
{
	*old_pwd = safe_getcwd(env);
	if (chdir(target) == -1)
	{
		perror("minishell");
		free(*old_pwd);
		return (1);
	}
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
	{
		*new_pwd = ft_strdup(target);
	}
	if (!*new_pwd)
	{
		perror("minishell");
		free(*old_pwd);
		return (1);
	}
	return (0);
}

/**
 * @brief Built-in command to change the current working directory.
 * This function handles the 'cd' command in the shell.
 * @param cmd The command structure containing the arguments.
 * @param env Pointer to the environment list.
 * @return 0 on success, 1 on failure.
 */
int	builtin_cd(t_cmd *cmd, t_env_list **env)
{
	char	*target;
	char	*old_pwd;
	char	*new_pwd;

	if (get_target_path(cmd, env, &target))
		return (1);
	if (perform_cd(target, &old_pwd, &new_pwd, env))
	{
		free(target);
		return (1);
	}
	free(target);
	if (old_pwd && update_var(env, "OLDPWD", old_pwd))
	{
		free_pwds(old_pwd, new_pwd);
		return (1);
	}
	if (update_var(env, "PWD", new_pwd))
	{
		free_pwds(old_pwd, new_pwd);
		return (1);
	}
	free_pwds(old_pwd, new_pwd);
	return (0);
}
