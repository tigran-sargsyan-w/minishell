/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:53:04 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/06 18:09:43 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define INITIAL_ARG_CAP 8

# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define RESET "\x1B[0m"

typedef struct s_cmd		t_cmd;
typedef enum e_redir_type	t_redir_type;
typedef struct s_redir		t_redir;

typedef struct s_cmd
{
	char					**args;
	int						arg_cap;
	t_redir					*in_redirs;
	t_redir					*out_redirs;
	struct s_cmd			*next;
}							t_cmd;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_HEREDOC,
	REDIR_OUT,
	REDIR_APPEND
}							t_redir_type;

typedef struct s_redir
{
	t_redir_type			type;
	char					*filename;
	int						quoted;
	struct s_redir			*next;
}							t_redir;

// parser.c
t_cmd						*parse_tokens(t_token *tokens, t_shell *sh);
void						print_cmds(t_cmd *cmd);
void						free_cmd_list(t_cmd *cmd);

// redir_utils.c
void						add_redirection(t_cmd *cmd, t_redir_type type,
								const char *filename, int quoted);

// expand_vars.c
char						*expand_vars(const char *input, t_shell *sh);
#endif