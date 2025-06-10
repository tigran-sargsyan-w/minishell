/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:42:50 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 23:55:33 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdlib.h>

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
