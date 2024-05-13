NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror
READLINE_FLAGS=-lreadline
ALL_FLAGS=$(CFLAGS) $(READLINE_FLAGS)

SRC_DIR=Sources/
SRC=$(SRC_DIR)main.c
OBJ=$(SRC:.c=.o)

$(NAME): $(OBJ)
	@make -C ./Libft/
	@cp ./Libft/libft.a .
	@ar -rc $(NAME) $(OBJ)
	@ranlib $(NAME)

.PHONY: all clean fclean re

all: $(NAME)
	@echo "[INFO] $(NAME) compiled"

run:
	@echo "[INFO] Executing Minishell..."
	$(CC) $(CFLAGS) $(SRC) libft.a -o Minishell
	@./Minishell

clean:
	@echo "[INFO] $(NAME) ${OBJ} removed"
	@rm -rf $(NAME) $(OBJ)
	@make clean -C ./Libft/

fclean: clean
	@echo "[INFO] Removing '*.a' files..."
	@rm -rf libft.a
	@make fclean -C ./Libft/

re: fclean all
