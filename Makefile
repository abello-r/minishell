RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
READLINE_FLAGS = -lreadline
ALL_FLAGS = $(CFLAGS) $(READLINE_FLAGS)

SRC_DIR = Sources
SRC_FILES = main.c error.c parse.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	@clear
	@make -C Libft
	@cp Libft/libft.a .
	@$(CC) $(ALL_FLAGS) $(OBJ) -o $(NAME) libft.a
	@echo "$(GREEN)[INFO]${NC} $(NAME) compiled$(NC)\n"

run: $(NAME)
	@clear
	@echo "$(GREEN)[INFO]${NC} Executing Minishell...$(NC)\n"
	@./$(NAME)

$(OBJ): $(SRC)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re run

all: $(NAME)

clean:
	@clear
	@echo "$(RED)[CLEAN]${NC} Removing object files...$(NC)\n"
	@rm -rf $(OBJ)
	@make clean -C Libft

fclean: clean
	@clear
	@echo "$(RED)[CLEAN]${NC} Removing $(NAME) and '*.a' files...$(NC)\n"
	@rm -rf $(NAME) libft.a
	@make fclean -C Libft

re: fclean all
