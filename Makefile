# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cscache <cscache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 18:25:47 by cscache           #+#    #+#              #
#    Updated: 2025/09/01 14:14:33 by cscache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0 -ggdb
LDFLAGS = -lreadline

NAME = minishell

# Directories
SRCS_DIR = sources
OBJS_DIR = obj
LIBFT_DIR = libft

# Libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I includes -I $(LIBFT_DIR)
HEADER = includes/minishell.h

# .c and .o
SRCS := $(shell find $(SRCS_DIR) -name '*.c')
OBJS := $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

# Default rule
all: $(NAME)

# Final exec
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

# .o
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Cleaning
clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

# Valgrind
valgrind: $(NAME)
	@echo "=== Valgrind with all leaks (including readline) ==="
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

valgrind-clean: $(NAME)
	@echo "=== Valgrind without readline leaks ==="
	-valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=no --track-origins=yes --suppressions=readline.supp ./$(NAME)

.PHONY: all clean fclean re valgrind valgrind-clean

# --show-leak-kinds=definite,indirect,possible 