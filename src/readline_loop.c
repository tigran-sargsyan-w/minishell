/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissemenov <denissemenov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/31 02:32:04 by denissemeno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

static int	is_only_whitespaces(char *input)
{
	while (*input)
	{
		if (is_space(*input) == 0)
			return (0);
		input++;
	}
	return (1);
}

void	readline_loop(t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmd;
	char	*input;

	setup_signal_handlers();
	while ((input = readline("minishell > ")) != NULL)
	{
		if (input[0] != '\0' && !is_only_whitespaces(input))
			// TODO: check if input is empty and if input is only spaces
		{
			if (ft_strncmp(input, "exit", 5) == 0)
			{
				free(input);
				break ;
			}
			add_history(input);
			tokens = lexer(input);
			print_tokens(tokens);
			if (tokens)
			{
				cmd = parse_tokens(tokens, sh);
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
