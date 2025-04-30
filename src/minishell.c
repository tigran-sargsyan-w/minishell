/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/30 13:09:46 by dsemenov         ###   ########.fr       */
=======
/*   Updated: 2025/04/29 18:07:02 by dsemenov         ###   ########.fr       */
>>>>>>> bda8b54813dc74e12ea42bd9423df4bff4e6779e
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input_str;

	(void)argc;
	(void)argv;
	while (envp[0] != NULL)
	{
		printf("%s\n", envp[0]);
		envp++;
	}
	(void)envp;
	input_str = NULL;
	readline_loop(&input_str);
	return (0);
}
