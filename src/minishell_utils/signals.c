/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:58:12 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 23:18:38 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t	g_signo = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * It sets a flag to indicate that the signal was received.
 * @param signo The signal number (not used in this handler).
 */
void	sigint_handler(int signo)
{
	(void)signo;
	rl_done = 1;
	g_signo = 1;
}

/**
 * @brief A dummy event handler that does nothing.
 * This is used to set the readline event hook to avoid unwanted behavior.
 * @return Always returns 0.
 */
int	event_nothing(void)
{
	return (0);
}

/**
 * @brief Sets up signal handlers for the shell.
 * It sets the SIGINT handler to `sigint_handler` and ignores SIGQUIT.
 * It also sets the readline event hook to `event_nothing`.
 */
void	setup_signal_handlers(void)
{
	rl_event_hook = event_nothing;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
