/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:19:56 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/20 11:06:05 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"
#include <stdlib.h>

static char	*get_from_env(char **envp, char *key);
static char	*build_command_path(char *dir, char *cmd);
static char	*search_in_paths(char **paths, char *cmd);
static void	free_array(char **array);

/**
 * @brief Retrieve the value associated with
 * a specified key from the environment variables.
 * @envp: An array of strings representing the environment variables.
 * @key: The name of the environment variable to search for.
 * @return Value associated with the key
 */
static char	*get_from_env(char **envp, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

/**
 * @brief Constructs a full command path string.
 * @param dir Directory in which the command is located.
 * @param cmd Command name.
 * @return A newly allocated string containing the full command path.
 */
static char	*build_command_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	path_len;

	path_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(path_len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, path_len);
	ft_strlcat(full_path, "/", path_len);
	ft_strlcat(full_path, cmd, path_len);
	return (full_path);
}

/**
 * @brief Searches for the command in the specified directories.
 * @param paths An array of strings representing the directories to search in.
 * @param cmd Command name.
 * @return A newly allocated string containing the full command path.
 */
static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_command_path(paths[i], cmd);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

/**
 * @brief Free the given array.
 * @param array The array to free.
 */
static void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * @brief Searches for the command in the specified directories.
 * @param cmd Command name.
 * @param envp An array of strings representing the environment variables.
 * @return A newly allocated string containing the full command path.
 */
char	*find_command(char *cmd, t_shell *sh)
{
	char		*path_env;
	char		**paths;

	if (!cmd || !sh->env_tab)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		handle_stat_errors(cmd, sh);
		if (access(cmd, R_OK) != 0 || access(cmd, X_OK) != 0)
		{
			ft_dprintf(2, "minishell: %s: Permission denied\n", cmd);
			sh->last_status = 126;
			free_all_resources(sh);
			exit(sh->last_status);
		}
		return (ft_strdup(cmd));
	}
	path_env = get_from_env(sh->env_tab, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
