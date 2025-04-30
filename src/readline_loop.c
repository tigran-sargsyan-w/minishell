/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/04/30 12:40:33 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	readline_loop(char **input)
{
	t_token	*tokens;

	while (1)
	{
		*input = readline("minishell > ");
		if (*input == NULL)
			break ;
		if ((*input)[0] != '\0')
		{
			if (ft_strncmp(*input, "exit", 5) == 0)
			{
				free(*input);
				break ;
			}
			add_history(*input);
			// TODO: Parse and execute
			tokens = lexer(*input);
			print_tokens(tokens);
			free_tokens(tokens);
		}
		free(*input);
	}
}
