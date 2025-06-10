/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:58:12 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 21:06:22 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t	g_signo = 0;

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
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
