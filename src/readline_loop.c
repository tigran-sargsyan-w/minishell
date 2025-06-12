/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/12 16:11:31 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

extern volatile sig_atomic_t	g_signo;

int	is_directory(const char *path)
{
	struct stat	sb;

	if (!path || (stat(path, &sb) == -1))
		return (0);
	return (S_ISDIR(sb.st_mode));
}

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

static void	process_input_line(char *input, t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmd;

	add_history(input);
	tokens = lexer(input);
	if (!tokens)
	{
		sh->last_status = 2;
		return ;
	}
	cmd = parse_tokens(tokens, sh);
	free_tokens(tokens);
	if (!cmd)
		return ;
	if (is_directory(cmd->args[0]))
	{
		ft_dprintf(2, "%s: Is a directory\n", cmd->args[0]);
		sh->last_status = 126;
	}
	else
		executor(cmd, sh);
}

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
	write(1, "exit\n", 5);
}
