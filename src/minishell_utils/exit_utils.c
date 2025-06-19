/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:45:05 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/18 14:39:39 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define FD_MAX 1024

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
