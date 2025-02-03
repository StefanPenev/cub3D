# **************************************************************************** #
#                                   Settings                                    #
# **************************************************************************** #

NAME            = cub3D
NAME_BONUS      = cub3D_bonus

CC              = cc
CFLAGS          = -Wall -Wextra -Werror -I./includes -I$(MLX_DIR) -g
CFLAGS_BONUS    = -Wall -Wextra -Werror -I./includes_bonus -I$(MLX_DIR) -g
LDFLAGS         = -L minilibx-linux -lmlx -lX11 -lXext -lm

LIBFT_DIR       = libft
LIBFT           = $(LIBFT_DIR)/libft.a

MLX_DIR         = minilibx-linux
MLX             = $(MLX_DIR)/libmlx.a

SRC_DIR         = src
SRC_BONUS_DIR   = src_bonus

OBJ_DIR         = obj
OBJ_BONUS_DIR   = obj_bonus

# Colors for pretty output
COLOR_RED       = \033[31m
COLOR_GREEN     = \033[32m
COLOR_BLUE      = \033[34m
COLOR_YELLOW    = \033[33m
COLOR_RESET     = \033[0m

# **************************************************************************** #
#                                  .c files                                     #
# **************************************************************************** #

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
		$(SRC_DIR)/init/init_square.c \
		$(SRC_DIR)/init/load_textures.c \

# Player
SRCS += $(SRC_DIR)/player/player.c \
		$(SRC_DIR)/player/player_movement.c \

# Raycaster
SRCS += $(SRC_DIR)/raycaster/debug.c \
		$(SRC_DIR)/raycaster/debug_utils.c \
		$(SRC_DIR)/raycaster/raycaster.c \
		$(SRC_DIR)/raycaster/raycaster_utils.c \
		$(SRC_DIR)/raycaster/raycast_utils.c \
		$(SRC_DIR)/raycaster/draw.c \

# Utils
SRCS += $(SRC_DIR)/utils/gnl.c \
		$(SRC_DIR)/utils/utils.c \
		$(SRC_DIR)/utils/map_utils.c \


# Bonus part
# Main
SRCS_BONUS += $(SRC_BONUS_DIR)/main.c \

# Cleanup
SRCS_BONUS += $(SRC_BONUS_DIR)/cleanup/game_cleanup.c \

# Map parsing
SRCS_BONUS += $(SRC_BONUS_DIR)/map_parsing/read_map.c \
			$(SRC_BONUS_DIR)/map_parsing/parse_map.c \
			$(SRC_BONUS_DIR)/map_parsing/flood_fill.c \
			$(SRC_BONUS_DIR)/map_parsing/error_checks.c \
			$(SRC_BONUS_DIR)/map_parsing/handle_colors.c \
			$(SRC_BONUS_DIR)/map_parsing/handle_textures.c \

# Init
SRCS_BONUS += $(SRC_BONUS_DIR)/init/init.c \
			$(SRC_BONUS_DIR)/init/init_hooks.c \
			$(SRC_BONUS_DIR)/init/init_square.c \
			$(SRC_BONUS_DIR)/init/load_textures.c \

# Player
SRCS_BONUS += $(SRC_BONUS_DIR)/player/player.c \
			$(SRC_BONUS_DIR)/player/player_movement.c \
			$(SRC_BONUS_DIR)/player/mouse_controls.c \

# Raycaster
SRCS_BONUS += $(SRC_BONUS_DIR)/raycaster/debug.c \
			$(SRC_BONUS_DIR)/raycaster/debug_utils.c \
			$(SRC_BONUS_DIR)/raycaster/raycaster.c \
			$(SRC_BONUS_DIR)/raycaster/raycaster_utils.c \
			$(SRC_BONUS_DIR)/raycaster/raycast_utils.c \
			$(SRC_BONUS_DIR)/raycaster/draw.c \

# Utils
SRCS_BONUS += $(SRC_BONUS_DIR)/utils/gnl.c \
			$(SRC_BONUS_DIR)/utils/utils.c \
			$(SRC_BONUS_DIR)/utils/map_utils.c \

# Anim
SRCS_BONUS += $(SRC_BONUS_DIR)/anim/doors.c \

# **************************************************************************** #
#                           Converting .c to .o                                 #
# **************************************************************************** #

# Convert .c paths to .o in their respective folders
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS  = $(SRCS_BONUS:$(SRC_BONUS_DIR)/%.c=$(OBJ_BONUS_DIR)/%.o)

# **************************************************************************** #
#                              Main targets                                    #
# **************************************************************************** #

all: $(NAME)

# Linking for the mandatory part
$(NAME): $(OBJS) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(COLOR_GREEN)Done! Mandatory executable built: $(NAME).$(COLOR_RESET)"

bonus: $(NAME_BONUS)

# Linking for the bonus part
$(NAME_BONUS): $(OBJS_BONUS) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS_BONUS) $(OBJS_BONUS) $(LIBFT) $(LDFLAGS) -o $(NAME_BONUS)
	@echo "$(COLOR_GREEN)Done! Bonus executable built: $(NAME_BONUS).$(COLOR_RESET)"

# **************************************************************************** #
#                         Rules for building object files                       #
# **************************************************************************** #

# Create .o files in obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Create .o files in obj_bonus directory
$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_BONUS) -c $< -o $@

# **************************************************************************** #
#                           Rules for building libraries                       #
# **************************************************************************** #

$(LIBFT):
	@make -C $(LIBFT_DIR) > /dev/null 2>&1

$(MLX):
	@make -C $(MLX_DIR) > /dev/null 2>&1

# **************************************************************************** #
#                                Clean targets                                  #
# **************************************************************************** #

clean:
	@rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@make -C $(MLX_DIR) clean > /dev/null 2>&1
	@echo "$(COLOR_YELLOW)All object files and directories have been removed.$(COLOR_RESET)"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@make -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@echo "$(COLOR_YELLOW)Executables and objects fully removed.$(COLOR_RESET)"

re: fclean all

rebonus: fclean bonus

# Mark these targets as phony to avoid name conflicts with files
.PHONY: all bonus clean fclean re rebonus
