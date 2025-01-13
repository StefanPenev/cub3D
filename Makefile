NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include
LDFLAGS_LINUX = -L minilibx-linux -lmlx -lX11 -lXext -lm
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx.a

SRCS = src/main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C Libft/

$(MLX):
	make -C $(MLX_DIR)/

clean:
	rm -f $(OBJS)
	make -C Libft/ clean
	make -C $(MLX_DIR)/ clean

fclean: clean
	rm -f $(NAME)
	make -C Libft/ fclean
	make -C $(MLX_DIR)/ clean

re: fclean all

.PHONY: all clean fclean re
