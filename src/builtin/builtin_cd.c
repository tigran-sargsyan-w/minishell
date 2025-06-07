/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsemenov <dsemenov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:51:42 by dsemenov          #+#    #+#             */
/*   Updated: 2025/06/07 02:35:23 by dsemenov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "libft.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	cd_no_args(t_env_list *old_pwd_node,
		t_env_list *new_pwd_node, char *old_pwd, t_env_list **env)
{
  t_env_list *home_node;
  char      *pwd;

  home_node = find_node_by_key(env, "HOME");
	if (home_node)
	{
		if (chdir(home_node->value) == -1)
    {
      free(old_pwd);
      perror("minishell");
      return (1);
    }
    pwd = getcwd(NULL, 0);
    if (!pwd)
    {
      free(old_pwd);
      perror("minishell");
      return (1);
    }
    if (old_pwd_node) 
      set_value(old_pwd_node, old_pwd);
    else
      export_argument("OLDPWD", old_pwd, env, EXPORT);
    free(old_pwd);
    if (new_pwd_node)
      set_value(new_pwd_node, pwd);
    else
      export_argument("PWD", pwd, env, EXPORT);
    free(pwd);
		return (0);
	}
	else
	{
    free(old_pwd);
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
}

int setup_pwds(t_env_list *old_pwd_node, t_env_list *new_pwd_node, char *old_pwd, t_env_list **env)
{
  char *new_pwd;

  new_pwd = getcwd(NULL, 0);
  if (!new_pwd)
  {
    free(old_pwd);
    perror("minishell");
    return (1);
  }
  if (old_pwd_node) 
    set_value(old_pwd_node, old_pwd);
  else
    export_argument("OLDPWD", old_pwd, env, EXPORT);
  free(old_pwd);
  if (new_pwd_node)
    set_value(new_pwd_node, new_pwd);
  else
    export_argument("PWD", new_pwd, env, EXPORT);
  free(new_pwd);
  return (0);
}

// READ MAN CD
int	builtin_cd(t_cmd *cmd, t_env_list **env)
{
	t_env_list	*old_pwd_node;
	t_env_list	*new_pwd_node;
	char		    *old_pwd;

	old_pwd_node = find_node_by_key(env, "OLDPWD");
	new_pwd_node = find_node_by_key(env, "PWD");
	old_pwd = getcwd(NULL, 0);
  if (!old_pwd)
  {
    perror("minishell");
    return (1);
  }
	if (cmd->args[2] != NULL)
	{
		ft_putendl_fd("minishell: Too many arguments", 2);
    free(old_pwd);
		return (1);
	}
	else if (cmd->args[1] == NULL)
		return (cd_no_args(old_pwd_node, new_pwd_node, old_pwd, env));
	if (chdir(cmd->args[1]) == -1)
	{
		perror("minishell");
    free(old_pwd);
		return (1);
	}
	else
    if (setup_pwds(old_pwd_node, new_pwd_node, old_pwd, env) == 1)
      return (1);
  return (0);
}
