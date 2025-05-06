/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:52:23 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/06 19:27:13 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "unistd.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int	builtin_pwd(t_cmd *cmd, t_env_list *env)
{
	(void)cmd;
	(void)env;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
