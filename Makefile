SRC_DIR = src

SRC_NAME = main.c
SRC_NAME += launch_command.c
SRC_NAME += ft_exit.c
SRC_NAME += str_utils.c
SRC_NAME += lst_utils.c
SRC_NAME += ft_split.c
SRC_NAME += rec_gnl.c
SRC_NAME += prm_init.c

SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))

OBJ_DIR = obj

OBJ_NAME = $(SRC_NAME:.c=.o)

OBJ = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))

NAME = minishell

INC = -I inc

CC = clang
RM = rm -f

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra

ifeq ($(d), 0)
	CFLAGS += -fsanitize=address
endif

all: $(MLX) $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	    $(CC) $(INC) $(CFLAGS) $(MLX_DEF) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $@

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(INC)

clean:
		$(RM) -r $(OBJ_DIR)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:	clean re fclean
