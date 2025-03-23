# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eunskim <eunskim@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 18:19:23 by eunskim           #+#    #+#              #
#    Updated: 2023/07/28 15:06:18 by eunskim          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#//= Colors =//#
BOLD	:= \033[1m
BLACK	:= \033[30;1m
RED		:= \033[31;1m
GREEN	:= \033[32;1m
YELLOW	:= \033[33;1m
BLUE	:= \033[34;1m
MAGENTA	:= \033[35;1m
CYAN	:= \033[36;1m
WHITE	:= \033[37;1m
RESET	:= \033[0m

#//= Mandatory part =//#
NAME			:= minishell
CFLAGS 			:= -Wall -Wextra -Werror
# -g3 -fsanitize=address

WHEREIS_BREW	:= if test -d /usr/local/opt/readline; then echo /usr/local; \
					elif test -d $(HOME)/.brew/opt/readline; then echo $(HOME)/.brew; \
					elif test -d $(HOME)/goinfre/.brew/opt/readline; then echo $(HOME)/goinfre/.brew; \
					elif test -d $(HOME)/homebrew/opt/readline; then echo $(HOME)/homebrew; \
					else echo ""; fi		
BREW_PATH		:= $(shell $(WHEREIS_BREW))

LIBFT_DIR		:= ./libft
LIBFT			:= $(LIBFT_DIR)/libft.a

LEXER_DIR		:= ./lexer
LEXER			:= $(LEXER_DIR)/lexer.a

PARSER_DIR		:= ./parser
PARSER			:= $(PARSER_DIR)/parser.a

INCLUDE_DIR		:= ./include
HEADERS 		:= -I $(BREW_PATH)/opt/readline/include -I $(LIBFT_DIR) -I $(INCLUDE_DIR)
LIBS			:= -lreadline -L $(BREW_PATH)/opt/readline/lib $(LIBFT) $(PARSER) $(LEXER)

SRCS	:= \
	src/main.c \
	src/signals.c \
	src/minishell_data_init.c \

EXPANDER_SRCS := \
	expander/expander.c \
	expander/expander_utils.c \
	expander/expander_here_end.c \
	expander/expander_executor_utils.c

EXECUTION_SRCS := \
	execution/execution.c \
	execution/assignments.c \
	execution/open_files.c \
	execution/here_doc.c \
	execution/redirections.c \
	execution/builtin_execution.c \
	execution/init_piper_data.c \
	execution/piper.c \
	execution/child_processes.c \
	execution/redirections_in_child.c \
	execution/get_envp.c \
	execution/get_right_path.c \
	execution/assignments_utils.c \
	execution/execution_utils.c \
	error_printer/error_printer.c \

BUILTIN_SRCS := \
	builtin/builtin_pwd.c \
	builtin/builtin_cd.c \
	builtin/builtin_echo.c \
	builtin/builtin_exit.c \
	builtin/builtin_exit_utils.c \
	builtin/builtin_env.c \
	builtin/builtin_export.c \
	builtin/builtin_unset.c \
	builtin/builtin_utils.c

OBJS := $(SRCS:.c=.o)
EXPANDER_OBJS := $(EXPANDER_SRCS:.c=.o)
EXECUTION_OBJS := $(EXECUTION_SRCS:.c=.o)
BUILTIN_OBJS := $(BUILTIN_SRCS:.c=.o)

#//= Make Rules =//#
all : libft lexer parser $(NAME)

libft:
	@$(MAKE) -C $(LIBFT_DIR)

lexer:
	@$(MAKE) -C $(LEXER_DIR)

parser:
	@$(MAKE) -C $(PARSER_DIR)

$(NAME)	: $(OBJS) $(EXPANDER_OBJS) $(EXECUTION_OBJS) $(BUILTIN_OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(EXPANDER_OBJS) $(EXECUTION_OBJS) $(BUILTIN_OBJS) -o $(NAME) $(LIBS) && \
	echo "$(YELLOW)>> Mandatory part - Minishell <<$(RESET)" && \
	echo "$(GREEN)Compilation successful!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(EXPANDER_OBJS) $(EXECUTION_OBJS) $(BUILTIN_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(PARSER_DIR) clean
	
fclean:	clean
	@rm -f $(NAME)
	@rm -f $(PARSER)
	@rm -f $(LEXER)
	@rm -f $(LIBFT)
	
re: fclean all

.PHONY: all clean fclean re libft lexer parser