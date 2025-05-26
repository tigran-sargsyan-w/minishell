/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/26 19:55:30 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "env.h"
#include <stdlib.h>
#include "executor.h"

static int  handle_exit_status(const char *input, size_t *i, char **res, t_shell *sh);
static int  handle_env_var(const char *input, size_t *i, char **res, t_shell *sh);
static int  handle_literal_dollar(const char *input, size_t *i, char **res);
static void append_char_to_result(const char *input, size_t *pos, char **result);
static char *get_env_value(const char *var_name, t_env_list *env_list);

static char *get_env_value(const char *var_name, t_env_list *env_list)
{
    t_env_list *tmp = env_list;
    size_t      len;

    while (tmp)
    {
        len = ft_strlen(tmp->key); 
        if (ft_strncmp(tmp->key, var_name, len + 1) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return ("");
}

// 1) $?
static int handle_exit_status(const char *input, size_t *pos, char **result, t_shell *sh)
{
    if (input[*pos + 1] != '?')
        return (FAILURE);

    char *status_str = ft_itoa(sh->last_status);
    if (!status_str)
        return (FAILURE);

    char *tmp = *result;
    *result = ft_strjoin(*result, status_str);
    free(tmp);
    free(status_str);

    *pos += 2;
    return (SUCCESS);
}

// 2) $NAME
static int handle_env_var(const char *input, size_t *pos, char **result, t_shell *sh)
{
    size_t j = *pos + 1;
    if (!(ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
        return (FAILURE);

    while (input[j] && (ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
        j++;

    char *var_name = ft_substr(input, *pos + 1, j - (*pos + 1));
    if (!var_name)
        return (FAILURE);

    char *val = get_env_value(var_name, sh->env_list);
    free(var_name);

    char *tmp = *result;
    *result = ft_strjoin(*result, val);
    free(tmp);

    *pos = j;
    return (SUCCESS);
}

// 3) just symbol '$'
static int handle_literal_dollar(const char *input, size_t *pos, char **result)
{
    char *tmp = *result;
    
    if (input[*pos] != '$')
        return (FAILURE);
    *result = ft_strjoin(tmp, "$");
    free(tmp);

    *pos += 1;
    return (SUCCESS);
}

static void append_char_to_result(const char *input, size_t *pos, char **result)
{
    char    buf[2];
    char   *tmp;

    buf[0] = input[*pos];
    buf[1] = '\0';
    tmp = *result;
    *result = ft_strjoin(*result, buf);
    free(tmp);
    (*pos)++;
}

char *expand_vars(const char *input, t_shell *sh)
{
    char   *result;
    size_t  pos;

    result = ft_strdup("");
    if (!result)
        return (NULL);

    pos = 0;
    while (input[pos])
    {
        if (input[pos] == '$')
        {
            if (handle_exit_status(input, &pos, &result, sh) == SUCCESS)
                continue;
            if (handle_env_var(input, &pos, &result, sh) == SUCCESS)
                continue;
            if (handle_literal_dollar(input, &pos, &result) == SUCCESS)
                continue;
        }
        else
        {
			append_char_to_result(input, &pos, &result);
        }
    }
    return (result);
}
