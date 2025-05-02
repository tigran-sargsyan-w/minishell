/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/01 16:56:56 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	readline_loop(char **input)
{
	t_token	*tokens;
	t_cmd	*cmd;

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
			cmd = parse_tokens(tokens);
			print_cmds(cmd);
			// TODO: Free cmd
			// TODO: Execute command
			free_tokens(tokens);
		}
		free(*input);
	}
}
