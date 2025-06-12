/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 22:02:56 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/12 05:01:54 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "ft_printf.h"
#include "libft.h"
#include "minishell.h"

static void	free_all_env(t_shell *sh)
{
	lst_clear(&sh->env_list);
	free_env_tab(sh->env_tab);
}

int	is_valid_numeric(char *arg)
{
	if (!(*arg))
		return (0);
	if ((*arg == '-' || *arg == '+') && ft_isdigit(arg[1]))
		arg++;
	if (ft_strlen(arg) > 18)
		return (0);
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (0);
		arg++;
	}
	return (1);
}

int	builtin_exit(t_shell *sh, char **argv)
{
	ft_dprintf(2, "exit\n");
	if (argv[1] == NULL)
	{
		free_all_env(sh);
		return (1);
	}
	if (!is_valid_numeric(argv[1]))
	{
		sh->last_status = 2;
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		free_all_env(sh);
		return (1);
	}
	if (argv[2])
	{
		sh->last_status = 1;
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (0);
	}
	sh->last_status = ft_atoi(argv[1]) % 256;
	free_all_env(sh);
	return (1);
}
