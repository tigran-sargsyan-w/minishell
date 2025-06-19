/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:08:16 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:55:45 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Retrieves the value of an environment variable by its name.
 * @param var_name The name of the environment variable.
 * @param env_list Pointer to the list of environment variables.
 * @return Returns the value of the environment variable, or an empty string
 * if not found.
 */
static char	*get_env_value(const char *var_name, t_env_list *env_list)
{
	t_env_list	*tmp;
	size_t		len;

	tmp = env_list;
	while (tmp)
	{
		len = ft_strlen(tmp->key);
		if (ft_strncmp(tmp->key, var_name, len + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return ("");
}

/**
 * @brief Handles the exit status variable ($?) in the input string.
 * @param input The input string containing the variable.
 * @param pos Pointer to the current position in the input string.
 * @param result Pointer to the result string where the value will be appended.
 * @param sh Pointer to the shell structure containing the last status.
 * @return Returns SUCCESS (0) on success, FAILURE (1) on failure.
 */
static int	handle_exit_status(const char *input, size_t *pos, char **result,
		t_shell *sh)
{
	char	*status_str;
	char	*tmp;

	if (input[*pos + 1] != '?')
		return (FAILURE);
	status_str = ft_itoa(sh->last_status);
	if (!status_str)
		return (FAILURE);
	tmp = *result;
	*result = ft_strjoin(*result, status_str);
	free(tmp);
	free(status_str);
	*pos += 2;
	return (SUCCESS);
}

/**
 * @brief Handles environment variable expansion in the input string.
 * @param input The input string containing the variable.
 * @param pos Pointer to the current position in the input string.
 * @param result Pointer to the result string where the value will be appended.
 * @param sh Pointer to the shell structure containing the environment list.
 * @return Returns SUCCESS (0) on success, FAILURE (1) on failure.
 */
static int	handle_env_var(const char *input, size_t *pos, char **result,
		t_shell *sh)
{
	size_t	j;
	char	*var_name;
	char	*val;
	char	*tmp;

	j = *pos + 1;
	if (!(ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
		return (FAILURE);
	while (input[j] && (ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
		j++;
	var_name = ft_substr(input, *pos + 1, j - (*pos + 1));
	if (!var_name)
		return (FAILURE);
	val = get_env_value(var_name, sh->env_list);
	free(var_name);
	tmp = *result;
	*result = ft_strjoin(*result, val);
	free(tmp);
	*pos = j;
	return (SUCCESS);
}

/**
 * @brief Appends a single character to the result string.
 * @param input The input string containing the character.
 * @param pos Pointer to the current position in the input string.
 * @param result Pointer to the result string, 
 * where the character will be appended.
 */
void	append_char_to_result(const char *input, size_t *pos, char **result)
{
	char	buf[2];
	char	*old;

	buf[0] = input[*pos];
	buf[1] = '\0';
	old = *result;
	*result = ft_strjoin(old, buf);
	free(old);
	(*pos)++;
}

/**
 * @brief Processes a dollar sequence in the input string.
 * @param input The input string containing the dollar sequence.
 * @param pos Pointer to the current position in the input string.
 * @param result Pointer to the result string, 
 * where the expanded value will be appended.
 * @param sh Pointer to the shell structure containing environment variables
 * and last status.
 * @return Returns 1 if a dollar sequence was processed, 0 otherwise.
 */
int	process_dollar_sequence(const char *input, size_t *pos,
		char **result, t_shell *sh)
{
	if (handle_exit_status(input, pos, result, sh) == SUCCESS)
		return (1);
	if (handle_env_var(input, pos, result, sh) == SUCCESS)
		return (1);
	if (!ft_isalpha(input[*pos + 1]) && input[*pos + 1] != '_'
		&& input[*pos + 1] != '?')
	{
		append_char_to_result(input, pos, result);
		return (1);
	}
	(*pos)++;
	return (0);
}
