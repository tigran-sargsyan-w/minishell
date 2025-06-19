/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/19 23:17:12 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "lexer.h"
#include "minishell.h"
#include "env.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

extern volatile sig_atomic_t	g_signo;

/**
 * @brief Checks if the given path is a directory.
 * @param path The path to check.
 * @return Returns 1 if the path is a directory, 0 otherwise.
 */
int	is_directory(const char *path)
{
	struct stat	sb;

	if (!path || (stat(path, &sb) == -1))
		return (0);
	return (S_ISDIR(sb.st_mode));
}

/**
 * @brief Handles the signal interrupt for the shell.
 * If the signal is SIGINT (Ctrl+C), it sets the last status to 130
 * and resets the signal number.
 * @param sh Pointer to the shell structure.
 * @return Returns 1 if the signal was handled, 0 otherwise.
 */
static int	handle_signal_interrupt(t_shell *sh)
{
	if (g_signo == 1)
	{
		if (sh->last_status < 128)
			sh->last_status = 130;
		g_signo = 0;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if the input string is empty or contains only whitespace.
 * @param input The input string to check.
 * @return Returns 1 if the input is empty or contains only whitespace,
 * 0 otherwise.
 */
static int	should_skip_input(const char *input)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Processes a single line of input from the user.
 * It adds the input to the history, tokenizes it, parses the tokens,
 * and executes the commands.
 * @param input The input string to process.
 * @param sh Pointer to the shell structure.
 */
static void	process_input_line(char *input, t_shell *sh)
{
	t_token	*tokens;

	add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		sh->last_status = 2;
		return ;
	}
	sh->cmd_list = parse_tokens(tokens, sh);
	free_tokens(tokens);
	if (!sh->cmd_list)
		return ;
	executor(sh);
	free_cmd_list(sh->cmd_list);
}

/**
 * @brief Reads lines from the user in a loop,
 * processes each line, and handles signals.
 * @param sh Pointer to the shell structure.
 */
void	readline_loop(t_shell *sh)
{
	char	*input;

	g_signo = 0;
	setup_signal_handlers();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (handle_signal_interrupt(sh))
		{
			free(input);
			continue ;
		}
		if (should_skip_input(input))
		{
			free(input);
			continue ;
		}
		process_input_line(input, sh);
		free(input);
		setup_signal_handlers();
	}
	clear_history();
	write(1, "exit\n", 5);
}
