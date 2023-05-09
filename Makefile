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

CC				= gcc

NAME			= pipex

BONUS_NAME		= pipex_bonus

CFLAGS			= -g3 -Wall -Wextra -Werror

SRCS			= child.c pipex.c utils.c

BONUS_SRCS		= bonus/child_bonus.c bonus/pipex_bonus.c bonus/utils_bonus.c

OBJS			= $(SRCS:.c=.o)

BONUS_OBJS		= $(BONUS_SRCS:.c=.o)

HEADER			= pipex.h

BONUS_HEADER	= bonus/pipex_bonus.h

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -g3 $(HEADER) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) -c -g3 $(CFLAGS) $< -o $@

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) -g3 $(BONUS_HEADER) $(BONUS_OBJS) -o $(BONUS_NAME)

clean:
	rm -f $(OBJS)
	rm -f $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)

re: fclean all

.PHONY: fclean clean all