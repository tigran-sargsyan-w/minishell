/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/16 17:51:12 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	readline_loop(t_env_list **env_variables)
{
	t_token	*tokens;
	t_cmd	*cmd;
	char	*input;

	while (1)
	{
		input = readline("minishell > ");
		if (input == NULL)
			break ;
		if (input[0] != '\0')
		{
			if (ft_strncmp(input, "exit", 5) == 0)
			{
				free(input);
				break ;
			}
			add_history(input);
			tokens = lexer(input);
			// print_tokens(tokens);
			if (tokens)
			{
				cmd = parse_tokens(tokens);
				// print_cmds(cmd);
			}
			if (cmd)
				executor(cmd, env_variables);
			free_tokens(tokens);
		}
		free(input);
	}
}
