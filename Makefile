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

CFLAGS	= -g #-Wall -Wextra -Werror

SRCS	= child.c pipex.c utils.c

OBJS	= $(SRCS:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -g pipex.h $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) -c -g $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: fclean clean all