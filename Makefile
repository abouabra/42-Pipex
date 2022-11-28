SRC = pipex.c

OBJS=$(SRC:.c=.o)
AR= ar -rcs
NAME=pipex
INCLUDE = -I. -I libft

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@make -C libft
	@$(CC) -Wall -Wextra -Werror $(INCLUDE) $^ libft/libft.a -o $@
	@printf "$(RED)--------PIPEX DONE--------$(RESET)\n"

#bonus: fclean all

%.o : %.c
	@printf "hana kan compili f$(GREEN) $< $(RESET) \n"
	@$(CC) -Wall -Wextra -Werror -c $<

#bonus: fclean all

clean:
	@make -C libft clean
	@rm -rf *.o $(LIBFT_OBJS)

fclean:
	@make -C libft fclean
	@rm -rf *.o $(NAME)

re:fclean all

.PHONY: all clean fclean re
