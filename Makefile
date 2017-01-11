# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncharret <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/05 19:16:41 by ncharret          #+#    #+#              #
#    Updated: 2017/01/11 12:48:46 by ncharret         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = abstract_vm
SRC = srcs/error_formatter.cc srcs/lexer.cc srcs/main.cc srcs/parser.cc srcs/ioperand_factory.cc srcs/machine.cc srcs/operand.cc
HEADERS = -I ./headers
OBJ = $(SRC:.cc=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@clang++ -g -stdlib=libc++ -std=c++11 -o $(NAME) $(OBJ) $(HEADERS)
	@echo "\\033[1;32m$(NAME) was created."

%.o: %.cc
	@clang++ -g -stdlib=libc++ -std=c++11 -Wall -Werror -Wextra -Wno-unused-parameter -o $@ -c $^ $(HEADERS)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
