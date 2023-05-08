# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: djacobs <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/01 17:08:06 by djacobs           #+#    #+#              #
#    Updated: 2023/04/01 17:08:07 by djacobs          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc

NAME	= pipex

CFLAGS	= -g3 #-Wall -Wextra -Werror

SRCS	= child_bonus.c pipex_bonus.c utils_bonus.c

OBJS	= $(SRCS:.c=.o)

HEADER	= pipex_bonus.h

all:$(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) -g3 $(HEADER) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) -c -g $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: fclean clean all