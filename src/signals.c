/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:58:12 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/23 22:58:28 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

// Handler for SIGINT (Ctrl-C)
static void	sigint_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	// Disable readline's default signal handling
	rl_catch_signals = 0;
	// Ctrl-C → our handler
	signal(SIGINT, sigint_handler);
	// Ctrl-\ → ignore
	signal(SIGQUIT, SIG_IGN);
}
