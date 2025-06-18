# Compiler and flags

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LD_FLAGS = -lreadline

# Directories

SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = include/
LIBFT_DIR = libft/

# Files

SRCS = minishell.c \
    	readline_loop.c \
		exit_utils.c \
		signals.c \
		free_utils.c \
		lexer/lexer.c \
		lexer/token_operators.c \
		lexer/token_utils.c \
		lexer/token_word.c \
		parser/cmd_args_builder.c \
		parser/expand_vars_utils.c \
		parser/expand_vars.c \
		parser/parser.c \
		parser/redir_utils.c \
		parser/token_utils.c \
		builtin/run_builtin.c \
		builtin/builtin_echo.c \
		builtin/builtin_cd.c \
		builtin/builtin_pwd.c \
		builtin/builtin_export.c \
		builtin/export_utils.c \
		builtin/builtin_unset.c \
		builtin/builtin_env.c \
		builtin/builtin_exit.c \
		builtin/builtin_utils.c \
		executor/executor_child.c \
		executor/executor.c \
		executor/find_command.c \
		executor/find_command_utils.c \
		executor/heredoc.c \
		executor/redirection.c \
		env/envp_list.c \
		env/env_list_to_tab.c \
		env/env_list_utils.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)%.o)
DEPS = $(OBJS:%.o=%.d)
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)libft.a

NAME = minishell

all: $(NAME)

libs:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) | libs
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LD_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all libs fclean clean re
