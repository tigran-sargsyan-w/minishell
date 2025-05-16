/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/16 15:28:55 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	executor(t_cmd *cmd, t_env_list **env_variables)
{
	int		saved_stdin;
	int		saved_stdout;
	char	**envp;

	envp = env_list_to_tab(env_variables);
	if (cmd->next == NULL)
	{
		// Single command with possible redirection
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		// Run builtin (or fall back to external)
		if (run_builtin(cmd, env_variables) == -1)
			execute_cmds(cmd, envp, env_variables);
		// Restore original fds
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	else
		execute_cmds(cmd, envp, env_variables);
	free_cmd_list(cmd);
}

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
