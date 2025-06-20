/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:02:24 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/20 10:33:04 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

static int	is_arg_n(const char *arg);
static void	print_args(char **argv);

/**
 * @brief Checks if the argument is a valid '-n' option.
 * This function checks if the argument starts with '-n'
 * and is followed by zero or more 'n' characters.
 * @param arg The argument to check.
 * @return 1 if the argument is a valid '-n' option, 0 otherwise.
 */
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

/**
 * @brief Prints the arguments to stdout, separated by spaces.
 * This function prints each argument in the provided array,
 * skipping empty arguments and ensuring that there is a space
 * between each printed argument.
 * @param argv The array of arguments to print.
 */
static void	print_args(char **argv)
{
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

/**
 * @brief Built-in command to print arguments to stdout.
 * This function handles the 'echo' command in the shell.
 * It checks for the '-n' option and prints the arguments accordingly.
 * If no arguments are provided, it prints a newline character.
 * @param cmd The command structure containing the arguments.
 * @param env Pointer to the environment list (not used in this function).
 * @return 0 on success, 1 on failure (not applicable here).
 */
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
		return (0);
	}
	print_args(argv);
	if (has_n_arg == 0)
		printf("\n");
	return (0);
}
