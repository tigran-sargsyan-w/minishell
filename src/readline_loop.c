/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/09 14:18:07 by tsargsya         ###   ########.fr       */
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

void	readline_loop(char **envp, t_env_list **env_variables)
{
	t_token	*tokens;
	t_cmd	*cmd;
	char *input;
	
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
			{
				if (cmd->next == NULL)
				{
					// Single command
					if (run_builtin(cmd, env_variables) == -1)
						execute_cmd(cmd, envp);
				}
				else
				{
					// Multiple commands (pipeline)
					execute_pipeline(cmd, envp, env_variables);
				}
				free_cmd_list(cmd);
			}
			free_tokens(tokens);
		}
		free(input);
	}
}
