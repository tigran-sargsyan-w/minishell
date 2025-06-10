/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:02:24 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/10 05:21:11 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "parser.h"
#include <stdio.h>

static int	is_arg_n(const char *arg)
{
	if ((arg[0] == '-') && (arg[1] == 'n'))
		arg++;
	else
		return (0);
	while (*arg)
	{
		if (*arg != 'n')
			return (0);
		arg++;
	}
	return (1);
}

static void	print_args(char **argv)
{
	if (*argv == NULL)
		printf("\n");
	while (*argv != NULL)
	{
		if (**argv == '\0')
		{
			argv++;
			continue ;
		}
		printf("%s", *argv);
		if (*(argv + 1) != NULL)
			printf(" ");
		argv++;
	}
}

int	builtin_echo(t_cmd *cmd, t_env_list **env)
{
	char	**argv;
	int		has_n_arg;

	(void)env;
	argv = cmd->args + 1;
	has_n_arg = 0;
	while (*argv && is_arg_n(argv[0]) == 1)
	{
		has_n_arg = 1;
		argv++;
	}
	if (*argv == NULL)
	{
		if (has_n_arg == 0)
			printf("\n");
		// TODO: check security
		return (0);
	}
	print_args(argv);
	if (has_n_arg == 0)
		printf("\n");
	// TODO: check security
	return (0);
}
