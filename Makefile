# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/06 21:25:05 by zel-fati          #+#    #+#              #
#    Updated: 2026/08/07 17:14:32 by zel-fati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC	= cc

CFLAGS	= -Wall -Wextra -Werror -pthread

NAME	= codexion

SRC	=	codex/main.c \
		codex/parsing_helper.c \
		codex/parsing_helper_2.c \
		codex/heap_init.c \
		codex/heap_func.c \
		codex/heap_utils.c \
		codex/sim_init.c \
		codex/acquire_dongles.c \
		codex/coder_routine.c \
		codex/monitor.c \
		codex/utils.c \
		codex/one_coder.c \
		codex/cleanup_and_utils.c

OBJ	= $(SRC:%.c=%.o)

HEADER	= codex/codexion.h

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean
