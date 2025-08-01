# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cscache <cscache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 16:36:11 by cscache           #+#    #+#              #
#    Updated: 2025/08/01 15:21:14 by cscache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = libft.a

INCLUDES = -I.

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c\
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_strlcpy.c\
		ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c\
		ft_strncmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_memmove.c\
		ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c\
		ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c\
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_memchr.c ft_memcmp.c\
		ft_printf.c ft_printf_utils.c get_next_line.c get_next_line_utils.c\
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_realloc.c\
		ft_lstlast.c ft_lstdelone.c ft_lstiter.c\
		ft_lstadd_back.c ft_lstclear.c ft_lstmap.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c libft.h ft_printf.h get_next_line.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
