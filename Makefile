# **************************************************************************** #
#                                  Makefile                                    #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LD_FLAGS = -lreadline

# -------------------------------
#  		  Directories 
# -------------------------------

SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = include/
LIBFT_DIR = libft/

# -------------------------------
#   Source for Minishell
# -------------------------------

SRCS = minishell.c \
    	minishell_utils/readline_loop.c \
		minishell_utils/exit_utils.c \
		minishell_utils/signals.c \
		minishell_utils/free_utils.c \
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

# -------------------------------
#   Object & Dependency Files
# -------------------------------
OBJS = $(SRCS:%.c=$(OBJ_DIR)%.o)
DEPS = $(OBJS:%.o=%.d)
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)libft.a

# **************************************************************************** #
#                                 Build Rules                                  #
# **************************************************************************** #

all: $(NAME)
	@echo "🔥 Minishell built successfully!"

libs:
	@$(MAKE) -s -C $(LIBFT_DIR)

$(NAME): $(OBJS) | libs
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LD_FLAGS) -o $(NAME)
	@echo "✅ Minishell object files compiled."
	@echo "🚀 $(NAME) created!"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "🗑️ $(NAME) object files removed."

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "😒 $(NAME) and $(NAME) object files removed."

re: fclean all

-include $(DEPS)

.PHONY: all libs fclean clean re
.DELETE_ON_ERROR: