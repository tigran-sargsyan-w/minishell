/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:45:05 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/19 23:12:26 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define FD_MAX 1024

/**
 * @brief Closes all file descriptors from 3 to FD_MAX.
 * This is useful for cleaning up resources before exiting the program.
 */
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

/**
 * @brief Prints an error message and exits the program with failure status.
 * @param msg The error message to print.
 */
void	error_exit(char *msg)
{
	perror(msg);
	close_all_fds();
	exit(EXIT_FAILURE);
}
