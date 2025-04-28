/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 17:30:28 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "tests.h"


int	main(int argc, char **argv, char **envp)
{
  char  *input_str;
	test_gnl();
	test_printf();
	test_libft();
	printf("\nWelcome to the shell!\n");
  input_str = readline("minishell > ");
  (void) argc;
  (void) argv;
  (void) envp;
	return (0);
}
