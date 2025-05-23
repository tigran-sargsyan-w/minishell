/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/23 22:37:25 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "libft.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	readline_loop(t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmd;
	char	*input;

	setup_signal_handlers();
	while ((input = readline("minishell > ")) != NULL)
	{
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
				executor(cmd, sh);
			free_tokens(tokens);
		}
		free(input);
	}
	// imitate Ctrl-D
	write(1, "exit\n", 5);
}
