/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/28 18:55:32 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
  char  *input_str;

  while (1)
  {
    input_str = readline("minishell > ");
    if (input_str == NULL)
      break ;
    if (input_str[0] != '\0')
    {
      if (ft_strncmp(input_str, "exit", 5) == 0)
      {
      free(input_str);
        break ;
      }
      add_history(input_str);
      // TODO: Parse and execute
    }
    free(input_str);
  }
  (void) argc;
  (void) argv;
  (void) envp;
	return (0);
}
