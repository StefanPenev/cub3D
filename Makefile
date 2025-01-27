NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -I$(MLX_DIR) -g
LDFLAGS = -L minilibx-linux -lmlx -lX11 -lXext -lm
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
SRC_DIR = src

# Main
SRCS += $(SRC_DIR)/main.c \

# Cleanup
SRCS += $(SRC_DIR)/cleanup/game_cleanup.c \

# Map parsing
SRCS += $(SRC_DIR)/map_parsing/read_map.c \
		$(SRC_DIR)/map_parsing/parse_map.c \
		$(SRC_DIR)/map_parsing/flood_fill.c \
		$(SRC_DIR)/map_parsing/error_checks.c \
		$(SRC_DIR)/map_parsing/handle_colors.c \
		$(SRC_DIR)/map_parsing/handle_textures.c \

# Init
SRCS += $(SRC_DIR)/init/init.c \
		$(SRC_DIR)/init/init_hooks.c \
		$(SRC_DIR)/init/load_textures.c \

# Player
SRCS += $(SRC_DIR)/player/player.c \
		$(SRC_DIR)/player/player_movement.c \

# Raycaster
SRCS += $(SRC_DIR)/raycaster/debug.c \
		$(SRC_DIR)/raycaster/debug_utils.c \
		$(SRC_DIR)/raycaster/raycaster.c \

# Utils
SRCS += $(SRC_DIR)/utils/gnl.c \
		$(SRC_DIR)/utils/utils.c \
		$(SRC_DIR)/utils/map_utils.c \
		
OBJS = $(SRCS:.c=.o)

COLOR_RED = \033[31m
COLOR_GREEN = \033[32m
COLOR_BLUE = \033[34m
COLOR_YELLOW = \033[33m
COLOR_RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "${COLOR_GREEN}Project compiled successfully.${COLOR_RESET}"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ 

$(LIBFT):
	@make -C libft/ > /dev/null 2>&1

$(MLX):
	@make -C $(MLX_DIR)/ > /dev/null 2>&1

clean:
	@rm -f $(OBJS)
	@make -C libft/ clean > /dev/null 2>&1
	@make -C $(MLX_DIR)/ clean > /dev/null 2>&1
	@echo "${COLOR_YELLOW}Object files and directories cleaned.${COLOR_RESET}"

fclean: clean
	@rm -f $(NAME) 
	@make -C libft/ fclean > /dev/null 2>&1
	@make -C $(MLX_DIR)/ clean > /dev/null 2>&1
	@echo "${COLOR_YELLOW}Executable and all objects fully cleaned.${COLOR_RESET}"

re: fclean all

.PHONY: all clean fclean re
