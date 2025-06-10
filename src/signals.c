/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:58:12 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 11:09:49 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t	g_signo = 0;

// Handler for SIGINT (Ctrl-C)
void	sigint_handler(int signo)
{
	(void)signo;
	rl_done = 1;
	g_signo = 1;
}

int	event_nothing(void)
{
	return (0);
}

void	setup_signal_handlers(void)
{
	rl_event_hook = event_nothing;
	// Ctrl-C → our handler
	signal(SIGINT, sigint_handler);
	// Ctrl-\ → ignore
	signal(SIGQUIT, SIG_IGN);
}
