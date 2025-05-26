/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/26 20:08:09 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

static int	handle_exit_status(const char *input, size_t *i, char **res,
				t_shell *sh);
static int	handle_env_var(const char *input, size_t *i, char **res,
				t_shell *sh);
static void	append_char_to_result(const char *input, size_t *pos,
				char **result);
static char	*get_env_value(const char *var_name, t_env_list *env_list);

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

// 1) $?
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

// 2) $NAME
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

static void	append_char_to_result(const char *input, size_t *pos, char **result)
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

char	*expand_vars(const char *input, t_shell *sh)
{
	char	*result;
	size_t	pos;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	pos = 0;
	while (input[pos])
	{
		if (input[pos] == '$')
		{
			if (handle_exit_status(input, &pos, &result, sh) == SUCCESS)
				continue ;
			if (handle_env_var(input, &pos, &result, sh) == SUCCESS)
				continue ;
			append_char_to_result(input, &pos, &result);
		}
		else
			append_char_to_result(input, &pos, &result);
	}
	return (result);
}
