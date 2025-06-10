#include "minishell.h"
#include "env.h"

void	clean_shell(t_shell *sh)
{
	if (!sh)
		return ;
	if (sh->env_tab)
		free_env_tab(sh->env_tab);
	if (sh->env_list)
		lst_clear(&sh->env_list);
}
