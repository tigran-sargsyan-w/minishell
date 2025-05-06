/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/06 14:47:28 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	readline_loop(char **input, char **envp)
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
			tokens = lexer(*input);
			// print_tokens(tokens);
			if (tokens)
			{
				cmd = parse_tokens(tokens);
				// print_cmds(cmd);
			}
			if (cmd)
			{
				run_builtin(cmd, lst_init(envp));
				execute_cmd(cmd, envp);
				free_cmd_list(cmd);
			}
			free_tokens(tokens);
		}
		free(*input);
	}
}
