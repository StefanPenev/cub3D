/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/20 16:36:33 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define WALL '1'
# define EMPTY '0'
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define WIDTH 800
# define HEIGHT 600
# define BLOCK 64

# define TILE_SIZE 64
# define EMPTY_TILE 0

# define TEX_WIDTH 128
# define TEX_HEIGHT 128

// # define CEILING_COLOR 0x87CEEB // Sky Blue
// # define FLOOR_COLOR   0x6E6E6E // Gray

# define MAP_COLOR 0x0000FF

typedef struct s_pos
{
	size_t			x;
	size_t			y;

}					t_pos;

typedef struct s_map
{
	char			**full_map;
	int				has_free_way;
	size_t			rows;
	size_t			columns;
	size_t			players_count;
	// t_pos	*players_positions;
	t_pos			player_position;
	char			orientation;
}					t_map;

typedef struct s_player
{
	float			x;
	float			y;
	float			angle;
	bool			key_up;
	bool			key_down;
	bool			key_left;
	bool			key_right;
	bool			left_rotate;
	bool			right_rotate;
}					t_player;

typedef struct s_texture
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	char *path; // anna
}					t_texture;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*img_data;
	int				bpp;
	int				size_line;
	int				endian;
	unsigned int	ceiling_color; //anna
	unsigned int	floor_color; //anna
	int				debug;
	t_player		player;
	t_texture		north_texture;
	t_texture		south_texture;
	t_texture		east_texture;
	t_texture		west_texture;
}					t_game;

typedef struct s_ctrl
{
	t_map			map;
	t_game			*game;
}					t_ctrl;

void				free_map(char **map, size_t rows);
void				game_cleanup(t_ctrl *ctrl);
void				clean_exit(char *str, t_ctrl *ctrl);
void				check_args(int argc, char *argv[], t_ctrl *ctrl);
void				check_valid_characters(t_ctrl *ctrl);
void				check_map_closed(t_ctrl *ctrl);
void				check_map_valid(t_ctrl *ctrl);
void				parse_map(char *filename, t_ctrl *ctrl);
void				find_biggest_line(t_ctrl *ctrl);
char				*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp);
int					hook_keypress(int keycode, t_ctrl *ctrl);
int					draw_loop(t_ctrl *ctrl);
int					key_release(int keycode, t_ctrl *ctrl);
int					key_press(int keycode, t_ctrl *ctrl);
void				init_player(t_player *player, float start_x, float start_y,
						float start_angle);
void				move_player(t_player *player, double delta_time,
						t_map *map);

#endif