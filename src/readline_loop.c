/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/04 19:08:33 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

static int	is_directory(char *cmd)
{
	size_t	len;

	if (!cmd)
		return (1);
	len = ft_strlen(cmd);
	if (((cmd[len - 1] == '.') || (cmd[len - 1] == '/')) || (cmd[0] == '.'
			&& cmd[1] == '\0'))
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", cmd);
		return (1);
	}
	return (0);
}

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
		{
			add_history(input);
			tokens = lexer(input);
			if (tokens)
			{
				// print_tokens(tokens);
				cmd = parse_tokens(tokens, sh);
				free_tokens(tokens);
				if (cmd && is_directory(cmd->args[0]))
				{
					sh->last_status = 126;
					free(input);
					continue ;
				}
				if (cmd)
				{
					// print_cmds(cmd);
					executor(cmd, sh);
				}
			}
		}
		free(input);
	}
	// imitate Ctrl-D
	write(1, "exit\n", 5);
}
