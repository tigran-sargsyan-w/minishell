/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:02:03 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/05 23:32:38 by dsemenov         ###   ########.fr       */
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
static int	is_directory(const char *path)
{
	struct stat	sb;

	if (!path || stat(path, &sb) == 1)
		return (0);
	if (S_ISDIR(sb.st_mode))
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", path);
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
		if (input[0] != '\0')
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
			else
			{
				sh->last_status = 2;
			}
		}
		free(input);
	}
	// imitate Ctrl-D
	write(1, "exit\n", 5);
}
