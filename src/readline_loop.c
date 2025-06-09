/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/09 11:58:58 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_printf.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>

extern volatile sig_atomic_t	g_signo;

// TODO: move this function to a more appropriate place?
int	is_directory(const char *path)
{
	struct stat	sb;

	if (!path || stat(path, &sb) == -1)
		return (0);
	if (S_ISDIR(sb.st_mode))
	{
		ft_dprintf(2, "%s: Is a directory\n", path);
		return (1);
	}
	return (0);
}

void	readline_loop(t_shell *sh)
{
	t_token	*tokens;
	t_cmd	*cmd;
	char	*input;

	g_signo = 0;
	setup_signal_handlers();
	while ((input = readline("minishell > ")) != NULL)
	{
		if (g_signo == 1)
		{
			if (sh->last_status < 128)
				sh->last_status = 130;
			g_signo = 0;
			free(input);
			continue ;
		}
		if (*input)
		{
			add_history(input);
			tokens = lexer(input);
			// print_tokens(tokens);
			if (tokens)
			{
				cmd = parse_tokens(tokens, sh);
				// print_cmds(cmd);
				free_tokens(tokens);
				if (cmd && is_directory(cmd->args[0]))
					sh->last_status = 126;
				else if (cmd)
					executor(cmd, sh);
			}
			else
				sh->last_status = 2;
		}
		free(input);
	}
	write(1, "exit\n", 5);
}
