/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:53:04 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/10 18:13:54 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "minishell.h"
# include <stddef.h>

# define INITIAL_ARG_CAP 8

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
void						free_cmd_list(t_cmd *cmd);

// redir_utils.c
void						add_redirection(t_cmd *cmd, t_redir_type type,
								const char *filename, int quoted);

// expand_vars.c
char						*expand_vars(const char *input, t_shell *sh);

// expand_vars_utils.c
void						append_char_to_result(const char *input,
								size_t *pos, char **result);
int							process_dollar_sequence(const char *input,
								size_t *pos, char **result, t_shell *sh);
#endif