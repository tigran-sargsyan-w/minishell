/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/30 12:34:13 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input_str;

	(void)argc;
	(void)argv;
	(void)envp;
	input_str = NULL;
	readline_loop(&input_str);
	return (0);
}
