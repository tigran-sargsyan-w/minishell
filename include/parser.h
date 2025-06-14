/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:53:04 by tsargsya          #+#    #+#             */
/*   Updated: 2025/06/14 20:23:16 by tsargsya         ###   ########.fr       */
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

// cmd_args_builder.c
t_cmd						*init_cmd(void);
int							append_arg(t_cmd *cmd, char *arg);
char						*build_argument(t_token **tokens, t_shell *sh);
char						*build_heredoc_delim(t_token **tokens, int *quoted,
								t_shell *sh);

// expand_vars.c
char						*expand_vars(const char *input, t_shell *sh);

// expand_vars_utils.c
void						append_char_to_result(const char *input,
								size_t *pos, char **result);
int							process_dollar_sequence(const char *input,
								size_t *pos, char **result, t_shell *sh);

// parser.c
t_cmd						*parse_tokens(t_token *tokens, t_shell *sh);

// redir_utils.c
void						apply_redirection(t_cmd *cmd, int redirect_type,
								const char *value, int quoted);
// token_utils.c
int							is_arg_token(t_token_type type);
int							is_redir_token(t_token_type type);
int							cmd_is_valid_before_pipe(t_cmd *cmd);
char						*get_chunk_for_token(t_token *tok, t_shell *sh);

#endif