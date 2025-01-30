/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/30 16:03:09 by anilchen         ###   ########.fr       */
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
# define DOOR 'D'
# define COLLECTIBLE 'C'

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define WIDTH 800
# define HEIGHT 600

# define TILE_SIZE 64
# define EMPTY_TILE 0

# define TEX_WIDTH 128
# define TEX_HEIGHT 128

# define MAP_COLOR 0x0000FF

// bonus

# define TIME_SPEED 5

# define MAX_FRAMES 3

# define MAX_DOORS 100

typedef struct s_anim
{
	size_t			fc;
	size_t			ac;
}					t_anim;

typedef struct s_pos
{
	size_t			x;
	size_t			y;
	float			orientation;
}					t_pos;

typedef struct s_gnl
{
	char			*save;
	char			*line;
}					t_gnl;

typedef struct s_point
{
	size_t			x;
	size_t			y;
}					t_point;

typedef struct s_map
{
	char			**full_map;
	char			*map_tmp;
	int				has_free_way;
	int				textures_defined;
	int				colors_defined;
	int				map_started;
	size_t			rows;
	size_t			columns;
	size_t			players_count;
	t_pos			player_position;
	t_pos			player_index;
	t_point doors[MAX_DOORS]; // bonus
	int doors_count;          // bonus
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
	char			*path;
	char			**frames;
	char			**paths;
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
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	int				debug;
	t_player		player;
	t_texture		north_texture;
	t_texture		south_texture;
	t_texture		east_texture;
	t_texture		west_texture;
	t_texture door; // bonus
}					t_game;

typedef struct s_ctrl
{
	t_map			map;
	t_game			*game;
	t_gnl			gnl;
	t_texture		texture;
	t_anim			anim;
}					t_ctrl;

typedef struct s_line_params
{
	int				delta_x;
	int				delta_y;
	int				step_x;
	int				step_y;
	int				error_val;
	int				error_val2;
}					t_line_params;

typedef struct s_raycast_debug
{
	float			ray_angle;
	int				hit;
	float			distance;
	size_t			grid_x;
	size_t			grid_y;
	float			ray_dir_x;
	float			ray_dir_y;
	float			delta_dist_x;
	float			delta_dist_y;
	int				step_x;
	int				step_y;
	float			side_dist_x;
	float			side_dist_y;
}					t_raycast_debug;

typedef struct s_raycast
{
	float			ray_angle;
	float			ray_dir_x;
	float			ray_dir_y;
	int				map_x;
	int				map_y;
	float			delta_dist_x;
	float			delta_dist_y;
	int				step_x;
	int				step_y;
	float			side_dist_x;
	float			side_dist_y;
	int				hit;
	int				side;
	float			real_dist;
	float			perp_dist;
	int				wall_height;
	int				draw_start;
	int				draw_end;
	float			hit_x;
	float			hit_y;
	float			wall_x;
	t_texture		*selected_texture;
	int				tex_x;
	float			step;
	float			tex_pos;
	int				y;
	int				tex_y;
	int				color;
	int is_door; //anna
}					t_raycast;

typedef struct s_square
{
	int				x;
	int				y;
	int				size;
	int				color;
}					t_square;

/* ************************************************************************** */
/*              				game_cleanup.c                                */
/* ************************************************************************** */

void				free_map(char **map, size_t rows);
void				game_cleanup(t_ctrl *ctrl);
void				clean_exit(char *str, t_ctrl *ctrl);
int					close_window(t_ctrl *ctrl);
int					close_window(t_ctrl *ctrl);

/* ************************************************************************** */
/*              				handle_colors.c                               */
/* ************************************************************************** */

int					is_color_definition(char *line_tmp);
void				validate_rgb_line(char *clean_line_tmp, char *line,
						t_ctrl *ctrl);
char				**split_and_check(char *line, t_ctrl *ctrl);
void				define_fc_colors(char *line, t_ctrl *ctrl);

/* ************************************************************************** */
/*              				handle_textures.c                             */
/* ************************************************************************** */

int					is_texture_definition(char *line_tmp);
void				keep_textures_path(char *line, t_ctrl *ctrl);

/* ************************************************************************** */
/*              					map_utils.c                               */
/* ************************************************************************** */

void				find_biggest_line(t_ctrl *ctrl);
char				*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp);
void				free_and_exit(char *msg, char *line, char **tmp_arr,
						t_ctrl *ctrl);
char				*remove_inner_spaces(char *str, t_ctrl *ctrl);

/* ************************************************************************** */
/*              					read_map.c                                */
/* ************************************************************************** */

char				*read_map(char *filename, t_ctrl *ctrl);

/* ************************************************************************** */
/*									utils.c									  */
/* ************************************************************************** */

char				*trim_trailing_whitespace(char *str);
int					ft_isspace(int c);

/* ************************************************************************** */
/*									player.c								  */
/* ************************************************************************** */

int					key_release(int keycode, t_ctrl *ctrl);
int					key_press(int keycode, t_ctrl *ctrl);
bool				in_map_bounds(float x, float y, t_map *map);

/* ************************************************************************** */
/*								player_movement.c  							  */
/* ************************************************************************** */

void				move_player(t_ctrl *ctrl, double delta_time);

/* ************************************************************************** */
/*									parse_map.c  							  */
/* ************************************************************************** */

void				parse_map(char *filename, t_ctrl *ctrl);

/* ************************************************************************** */
/*										gnl.c  								  */
/* ************************************************************************** */

char				*gnl(int fd, t_ctrl *ctrl);

/* ************************************************************************** */
/*								error_checks.c  							*/
/* ************************************************************************** */

void				check_args(int argc, char *argv[], t_ctrl *ctrl);
void				check_valid_characters(t_ctrl *ctrl);
void				check_map_closed(t_ctrl *ctrl);

/* ************************************************************************** */
/*									flood_fill.c  							  */
/* ************************************************************************** */

void				check_map_valid(t_ctrl *ctrl);

/* ************************************************************************** */
/*              				raycaster.c                                   */
/* ************************************************************************** */

void				handle_rays(t_ctrl *ctrl, float start_angle,
						float angle_step);

/* ************************************************************************** */
/*              				raycaster_utils.c                             */
/* ************************************************************************** */

double				compute_delta_time(void);
void				normalize_angle(float *angle);
int					clamp(int value, int min, int max);
bool				touch(size_t grid_x, size_t grid_y, t_map *map);
int					get_texture_color(t_texture *texture, int tex_x, int tex_y);

/* ************************************************************************** */
/*              				raycast_utils.c                               */
/* ************************************************************************** */

void				compute_wall_x(t_raycast *rc);
void				choose_texture(t_raycast *rc, t_ctrl *ctrl);
void				draw_wall(t_game *gm, t_raycast *rc, int col);
void				draw_ceil_floor(t_game *gm, t_raycast *rc, int col);
void				compute_wall_dimensions(t_raycast *rc, t_player *pl);


/* ************************************************************************** */
/*              					debug.c                                   */
/* ************************************************************************** */

void				draw_debug(t_ctrl *ctrl);

/* ************************************************************************** */
/*              					debug_utils.c                             */
/* ************************************************************************** */

void				ray_step_loop(t_raycast_debug *rc, t_map *map);
void				init_raycast_data(t_raycast_debug *rc, t_game *game);
void				draw_line_coords(int block_size, int x_end, int y_end,
						t_game *game);

/* ************************************************************************** */
/*              						draw.c                                */
/* ************************************************************************** */

int					draw_loop(t_ctrl *ctrl);
void				draw_map(t_map *mapp, t_game *game);
void				put_pixel(int x, int y, int color, t_game *game);
void				draw_square(t_square square, t_game *game);

/* ************************************************************************** */
/*              					init.c                                    */
/* ************************************************************************** */

int					init_ctrl(t_ctrl *ctrl);
void				init_player(t_player *player, float start_x, float start_y,
						float orientation);
t_square			init_square(int x, int y, int size, int color);

/* ************************************************************************** */
/*              				load_textures.c                               */
/* ************************************************************************** */

void				load_all_textures(t_game *game, t_ctrl *ctrl);

/* ************************************************************************** */
/*              					init_hooks.c                              */
/* ************************************************************************** */

void				init_hooks(t_ctrl *ctrl);
void				init_game_window(t_ctrl *ctrl);

// bonus
/* ************************************************************************** */
/*              						anim.c   							  */
/* ************************************************************************** */

void				select_frame(t_ctrl *ctrl);

int					mouse_move(int x, int y, t_ctrl *ctrl);

#endif