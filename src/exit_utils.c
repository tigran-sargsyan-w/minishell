/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:45:05 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/17 20:18:05 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define FD_MAX 8192

void	close_all_fds(void)
{
	int			fd;
	struct stat	st;

	fd = 3;
	while (fd < FD_MAX)
	{
		if (fstat(fd, &st) != -1 || errno != EBADF)
			close(fd);
		fd++;
	}
}

void	error_exit(char *msg)
{
	perror(msg);
	close_all_fds();
	exit(EXIT_FAILURE);
}
