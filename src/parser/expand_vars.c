/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/26 19:29:11 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "env.h"
#include <stdlib.h>

static int  handle_exit_status(const char *input, size_t *i, char **res, t_shell *sh);
static int  handle_env_var(const char *input, size_t *i, char **res, t_shell *sh);
static int  handle_literal_dollar(const char *input, size_t *i, char **res);
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

// Главная функция — сканирует строку и вызывает хендлеры:
char *expand_vars(const char *input, t_shell *sh)
{
    char   *result;
    size_t  i;

    result = ft_strdup("");
    if (!result)
        return (NULL);

    i = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            if (handle_exit_status(input, &i, &result, sh))
                continue;
            if (handle_env_var(input, &i, &result, sh))
                continue;
            if (handle_literal_dollar(input, &i, &result))
                continue;
        }
        else
        {
            // копируем обычный символ
            char buf[2] = { input[i], '\0' };
            char *tmp   = result;

            result = ft_strjoin(result, buf);
            free(tmp);
            i++;
        }
    }
    return (result);
}

// 1) $?
static int handle_exit_status(const char *input, size_t *i, char **res, t_shell *sh)
{
    if (input[*i + 1] != '?')
        return (0);

    char *status_str = ft_itoa(sh->last_status);
    if (!status_str)
        return (0);

    // result = result + status_str
    char *tmp = *res;
    *res = ft_strjoin(*res, status_str);
    free(tmp);
    free(status_str);

    *i += 2;
    return (1);
}

// 2) $NAME
static int handle_env_var(const char *input, size_t *i, char **res, t_shell *sh)
{
    size_t j = *i + 1;

    // проверяем, что за $ идёт допустимый первый символ имени
    if (!(ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
        return (0);

    // ищем конец имени
    while (input[j] && (ft_isalnum((unsigned char)input[j]) || input[j] == '_'))
        j++;

    // вырезаем имя
    char *var_name = ft_substr(input, *i + 1, j - (*i + 1));
    if (!var_name)
        return (0);

    // ищем в вашем списке env
    char *val = get_env_value(var_name, sh->env_list);
    free(var_name);

    // конкатенируем значение
    char *tmp = *res;
    *res = ft_strjoin(*res, val);
    free(tmp);

    *i = j;
    return (1);
}

// 3) просто литерал '$'
static int handle_literal_dollar(const char *input, size_t *i, char **res)
{
    (void)input;
    // здесь мы гарантированно на input[*i] == '$'
    char *tmp = *res;
    *res = ft_strjoin(*res, "$");
    free(tmp);

    *i += 1;
    return (1);
}
