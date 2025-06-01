NAME := philo
CC := cc
CFLAGS := -Wall -Wextra -Werror

sources = \
		main.c \
		simulation_init.c \
		simulation_parse.c \

object = $(sources:.c=.o)

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(object)
	@$(CC) $(CFLAGS) $(object) -o $(NAME)
	@echo "\033[32m	Philosophers Compiled Successfully! ✅\033[0m"

clean:
	@rm -rf $(object)
	@echo "\033[33m	Object Files Removed! 🗑️\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[31m	Executable removed! ❌\033[0m"

re: fclean all

flush: re clean
	@rm -rf $(object)

f: $(object)
	@$(CC) $(CFLAGS) -o $(NAME)
	@rm -rf $(object)
	@echo "\033[32m	Ready To Execute! ✅\033[0m"

.SECONDARY: $(object)
.PHONY: clean fclean re all
