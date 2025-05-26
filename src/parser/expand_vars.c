/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/26 18:02:22 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "env.h"
#include <stdlib.h>

// Ищет переменную var_name в списке env_list.
// Если находит — возвращает её value, иначе — пустую строку.
char *get_env_value(const char *var_name, t_env_list *env_list)
{
    t_env_list *tmp = env_list;
    size_t      len;

    while (tmp)
    {
        len = ft_strlen(tmp->key);
        // сравниваем полное совпадение ключа   
        if (ft_strncmp(tmp->key, var_name, len + 1) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return "";  // переменной нет — будем добавлять пустое
}

// Раскрывает все $VAR и $? внутри строки input.
// input — нуль-терминированная строка без внешних кавычек.
// Возвращает новую строку, которую нужно free() по завершении.
char *expand_vars(const char *input, t_shell *sh)
{
    char    *result;
    size_t   i;

    // Инициализируем результат пустой строкой
    result = ft_strdup("");
    if (!result)
        return (NULL);

    i = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            // 1) Спец-переменная $?
            if (input[i + 1] == '?')
            {
                char *status_str;
                char *tmp;

                status_str = ft_itoa(sh->last_status);
                if (!status_str)
                    return (free(result), NULL);

                tmp = result;
                result = ft_strjoin(result, status_str);
                free(tmp);
                free(status_str);

                i += 2;
            }
            // 2) Обычная переменная $NAME
            else if (ft_isalnum((unsigned char)input[i + 1]) 
                     || input[i + 1] == '_')
            {
                size_t j = i + 1;
                char  *var_name;
                char  *val;
                char  *tmp;

                // найдём конец имени переменной
                while (input[j] &&
                       (ft_isalnum((unsigned char)input[j]) 
                        || input[j] == '_'))
                    j++;

                // вырезаем имя переменной
                var_name = ft_substr(input, i + 1, j - (i + 1));
                if (!var_name)
                    return (free(result), NULL);

                // смотрим в вашем списке env
                val = get_env_value(var_name, sh->env_list);
                free(var_name);

                // добавляем значение (или пустую строку)
                tmp = result;
                result = ft_strjoin(result, val);
                free(tmp);

                i = j;
            }
            // 3) не $?, не $LETTER — просто литерал '$'
            else
            {
                char *tmp;

                tmp = result;
                result = ft_strjoin(result, "$");
                free(tmp);
                i++;
            }
        }
        else
        {
            // Все прочие символы — копируем «как есть»
            char buf[2];
            char *tmp;

            buf[0] = input[i];
            buf[1] = '\0';
            tmp = result;
            result = ft_strjoin(result, buf);
            free(tmp);
            i++;
        }
    }

    return (result);
}


