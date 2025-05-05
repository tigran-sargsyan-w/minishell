/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:02:24 by dsemenov          #+#    #+#             */
/*   Updated: 2025/05/05 17:36:52 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdio.h>

static void	print_env(char *var, t_env_list *env)
{
	t_env_list	*tmp_env;
	size_t		len;

	tmp_env = env;
	while (tmp_env)
	{
		len = ft_strlen(tmp_env->key);
		if (ft_strncmp(tmp_env->key, var, len + 1) == 0)
		{
			printf("%s", tmp_env->value);
			break ;
		}
		tmp_env = tmp_env->next;
	}
}

int	builtin_echo(t_cmd *cmd, t_env_list *env)
{
	char **argv;
	int has_n_arg;

	argv = cmd->args + 1;
	has_n_arg = 0;
	while (*argv && (ft_strncmp(argv[0], "-n", 3) == 0))
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
	while (*argv != NULL)
	{
		if ((*argv)[0] == '$')
			print_env(*argv + 1, env);
		else
			printf("%s", *argv);
		if (*(argv + 1) != NULL)
			printf(" ");
		argv++;
	}
	if (has_n_arg == 0)
		printf("\n");
	return (0);
}