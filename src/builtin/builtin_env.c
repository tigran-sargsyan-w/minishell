/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:59 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/07 14:28:47 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int	builtin_env(t_cmd *cmd, t_env_list **env)
{
	(void)cmd;
	
	while (*env)
	{
		printf("%s=%s\n", (*env)->key, (*env)->value);
		*env = (*env)->next;
	}
	return (EXIT_SUCCESS);
}
