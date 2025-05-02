/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:53:04 by tsargsya          #+#    #+#             */
/*   Updated: 2025/05/02 12:15:53 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

# define INITIAL_ARG_CAP 8

typedef struct s_cmd	t_cmd;

/**
 * @brief Structure for command
 * @param args Array of arguments
 * @param infile Input file for redirection
 * @param outfile Output file for redirection
 * @param append 1 for >>, 0 for >
 * @param heredoc 1 for <<, 0 for <
 * @param next Pointer to the next command (for pipes)
 */
typedef struct s_cmd
{
	char				**args;
	int					arg_cap;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	struct s_cmd		*next;
}						t_cmd;

// parser.c
t_cmd					*parse_tokens(t_token *tokens);
void					print_cmds(t_cmd *cmd);
void					free_cmd_list(t_cmd *cmd);
#endif