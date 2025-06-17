/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:45:05 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/17 20:00:07 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FD_MAX 1024

void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < FD_MAX)
	{
		close(i);
		i++;
	}
}

void	error_exit(char *msg)
{
	perror(msg);
	close_all_fds();
	exit(EXIT_FAILURE);
}
