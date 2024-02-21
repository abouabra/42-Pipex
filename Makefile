SRC = pipex.c

NAME=pipex
INCLUDE = -I. -I libft

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

all: ascii $(NAME)

$(NAME): $(SRC)
	@make -C libft > /dev/null
	@$(CC) -Wall -Wextra -Werror $(INCLUDE) $^ libft/libft.a -o $@

clean:
	@make -C libft clean > /dev/null
	@rm -rf *.o

fclean:
	@make -C libft fclean > /dev/null
	@rm -rf *.o $(NAME)

re:fclean all

ascii:
	@clear
	@printf "$(GREEN)\n\
																					 \n\
PPPPPPPPPPPPPPPPP     iiii                                                           \n\
P::::::::::::::::P   i::::i                                                          \n\
P::::::PPPPPP:::::P   iiii                                                           \n\
PP:::::P     P:::::P                                                                 \n\
  P::::P     P:::::Piiiiiiippppp   ppppppppp       eeeeeeeeeeee  xxxxxxx      xxxxxxx\n\
  P::::P     P:::::Pi:::::ip::::ppp:::::::::p    ee::::::::::::ee x:::::x    x:::::x \n\
  P::::PPPPPP:::::P  i::::ip:::::::::::::::::p  e::::::eeeee:::::eex:::::x  x:::::x  \n\
  P:::::::::::::PP   i::::ipp::::::ppppp::::::pe::::::e     e:::::e x:::::xx:::::x   \n\
  P::::PPPPPPPPP     i::::i p:::::p     p:::::pe:::::::eeeee::::::e  x::::::::::x    \n\
  P::::P             i::::i p:::::p     p:::::pe:::::::::::::::::e    x::::::::x     \n\
  P::::P             i::::i p:::::p     p:::::pe::::::eeeeeeeeeee     x::::::::x     \n\
  P::::P             i::::i p:::::p    p::::::pe:::::::e             x::::::::::x    \n\
PP::::::PP          i::::::ip:::::ppppp:::::::pe::::::::e           x:::::xx:::::x   \n\
P::::::::P          i::::::ip::::::::::::::::p  e::::::::eeeeeeee  x:::::x  x:::::x  \n\
P::::::::P          i::::::ip::::::::::::::pp    ee:::::::::::::e x:::::x    x:::::x \n\
PPPPPPPPPP          iiiiiiiip::::::pppppppp        eeeeeeeeeeeeeexxxxxxx      xxxxxxx\n\
                            p:::::p                                                  \n\
                            p:::::p                                                  \n\
                           p:::::::p                        $(RED)by: abouabra $(GREEN)                         \n\
                           p:::::::p                                                 \n\
                           p:::::::p                                                 \n\
                           ppppppppp                                                 \n\
                                                                             $(RESET)\n"
.PHONY: all clean fclean re
