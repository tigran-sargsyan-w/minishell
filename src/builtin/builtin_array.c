#include "builtins.h"
#include "libft.h"
#include "stddef.h"

int	run_builtin(t_cmd *cmd, t_env_list *env)
{
	(void)env;

	int i;
	static const t_builtins_array builtins_arr[] = {{"echo", builtin_echo},
		{"cd", builtin_cd}, {"pwd", builtin_pwd}, {"env", builtin_env},
		{"export", builtin_export}, {"unset", builtin_unset}, {"exit",
		builtin_exit}, {NULL, NULL}};

	i = 0;
	while (builtins_arr[i].builtin_name != NULL)
	{
		if (ft_strncmp(cmd->args[0], builtins_arr[i].builtin_name,
				(ft_strlen(cmd->args[0])) + 1) == 0)
		{
			builtins_arr[i].handler(cmd, env);
			break ;
		}
		i++;
	}
	return (-1);
}