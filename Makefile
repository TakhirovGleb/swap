NAME = push_swap
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iinclude
COMMON = utils stack names parse rank sort
SRC = $(addprefix src/,$(addsuffix .c,$(COMMON) medium bench main))
OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(addprefix src/,$(addsuffix .o,$(COMMON) checker_bonus))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

checker: $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $@

bonus: checker

src/%.o: src/%.c include/push_swap.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME) checker

re: fclean
	$(MAKE) all

.PHONY: all bonus clean fclean re
