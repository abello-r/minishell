COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = Sources
SRC_FILES = main.c error.c parse.c signal_handler.c Builtins/pwd.c Builtins/env.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ_DIR = Objects
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

ifeq ($(shell whoami), abello-r)
	RLHEADER = -I "/opt/homebrew/opt/readline/include"
	LIBS = -L "/opt/homebrew/opt/readline/lib" -lreadline
else
	RLHEADER = -I "/Users/$(USER)/.brew/opt/readline/include"
	LIBS = -L "/Users/$(USER)/.brew/opt/readline/lib" -lreadline
endif

$(NAME): $(OBJ)
	@clear
	@make -C Libft
	@cp Libft/libft.a .
	@$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o $(NAME) libft.a $(READLINE_FLAGS)
	@echo "$(COLOR_GREEN)[INFO]$(COLOR_RESET) $(NAME) compiled$(COLOR_RESET)\n"

run: $(NAME)
	@clear
	@echo "$(COLOR_GREEN)[INFO]$(COLOR_RESET) Executing Minishell...$(COLOR_RESET)\n"
	@./$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@ $(RLHEADER)

.PHONY: all clean fclean re run

all: $(NAME)

clean:
	@clear
	@echo "$(COLOR_RED)[CLEAN]$(COLOR_RESET) Removing object files...$(COLOR_RESET)\n"
	@$(RM) -rf $(OBJ_DIR)
	@make clean -C Libft

fclean: clean
	@clear
	@echo "$(COLOR_RED)[CLEAN]$(COLOR_RESET) Removing $(NAME) and '*.a' files...$(COLOR_RESET)\n"
	@$(RM) -f $(NAME) libft.a
	@make fclean -C Libft

re: fclean all
