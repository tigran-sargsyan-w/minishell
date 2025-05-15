# Compiler and flags

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
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
		parser/lexer.c \
		parser/lexer_utils.c \
		parser/parser.c \
		builtin/run_builtin.c \
		builtin/builtin_echo.c \
		builtin/builtin_cd.c \
		builtin/builtin_pwd.c \
		builtin/builtin_export.c \
		builtin/builtin_unset.c \
		builtin/builtin_env.c \
		builtin/builtin_exit.c \
		executor/executor.c \
		executor/executor_utils.c \
		executor/find_command.c \
		env/envp_list.c \
		env/env_list_to_tab.c

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
