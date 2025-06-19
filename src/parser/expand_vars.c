/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 22:58:29 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdlib.h>

/**
 * @brief Expands variables in the input string.
 * @param input The input string containing variables to expand.
 * @param sh Pointer to the shell structure containing environment variables.
 * @param had_content Pointer to an integer that will be set to 1 
 * if any content is added to the result.
 * @return Returns the expanded string, or NULL on failure.
 */
static char	*do_expand_vars(const char *input, t_shell *sh, int *had_content)
{
	char	*result;
	size_t	pos;

	result = ft_strdup("");
	pos = 0;
	if (!result)
		return (NULL);
	*had_content = 0;
	while (input[pos])
	{
		if (input[pos] == '$')
		{
			if (process_dollar_sequence(input, &pos, &result, sh))
			{
				*had_content = 1;
				continue ;
			}
		}
		else
		{
			append_char_to_result(input, &pos, &result);
			*had_content = 1;
		}
	}
	return (result);
}

/**
 * @brief Expands variables in the input string and handles special cases.
 * @param input The input string containing variables to expand.
 * @param sh Pointer to the shell structure containing environment variables.
 * @return Returns the expanded string, or NULL if no content was added 
 * or if the input was just a dollar sign ($).
 */
char	*expand_vars(const char *input, t_shell *sh)
{
	int		had_content;
	char	*result;

	result = do_expand_vars(input, sh, &had_content);
	if (!result)
		return (NULL);
	if (!had_content || (result[0] == '\0' && input[0] == '$'))
	{
		free(result);
		return (NULL);
	}
	return (result);
}
