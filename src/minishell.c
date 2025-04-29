/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 20:35:00 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
  char  *input_str;
	t_token	*tokens;

  (void) argc;
  (void) argv;
  (void) envp;
  input_str = NULL;
  readline_loop(&input_str);
	tokens = lexer(input_str);
  print_tokens(tokens);
	free_tokens(tokens);
	free(input_str);
	return (0);
}
