/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 16:57:13 by anilchen         ###   ########.fr       */
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

// colors
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_RESET "\033[0m"
# define COLOR_BLUE "\033[34m"
# define COLOR_YELLOW "\033[33m"

// map values
# define WALL '1'
# define EMPTY '0'
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'
# define DOOR 'D'
# define CORPSE 'C'
# define ENEMY 'M'

// keys
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_E 101
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define MOUSE_CLICK 1
# define SPACE 32

# define WIDTH 800
# define HEIGHT 600

# define TILE_SIZE 64
# define EMPTY_TILE 0

# define TEX_WIDTH 128
# define TEX_HEIGHT 128

# define MAP_COLOR 0x0000FF

// bonus
# define TRANSPARENT_COLOR 0xFF00FF

# define TIME_SPEED 100

# define MAX_FRAMES 3

// door states
# define DOOR_CLOSED 0
# define DOOR_OPENING 1
# define DOOR_OPEN 2
# define DOOR_CLOSING 3

// box states
# define NOT_DAMAGED 0
# define DAMAGED 1
# define DESTROYED 2

// door direction
# define VERTICAL 0
# define HORIZONTAL 1

// trig tables
# define ANGLE_TABLE_SIZE 3600

// fight
# define PLAYER_HP 100
# define PLAYER_DAMAGE 25
# define ENEMY_HP 100
# define ENEMY_DAMAGE 20

typedef enum enemy_state
{
	ENEMY_IDLE,        // Default idle animation
	ENEMY_TRIGGERED,   // Play 5-frame alert animation once
	ENEMY_ACTIVE,      // Loop 2-frame animation while player is in range
	ENEMY_RETURN_IDLE, // Transition back to idle when player leaves
}					t_enemy_state;

typedef struct s_door
{
	int				x;
	int				y;
	double			timer;
	int				state;
}					t_door;

typedef struct s_fight
{
	int				fight_started;
	int				enemy_shoot;
	int				player_shoot;
	int				shoot_delay;
	int				lose_flag;
	int				win_flag;
}					t_fight;

typedef struct s_enemy
{
	float			x;
	float			y;
	float			distance;
	int				state;
	int				frame;
	float			frame_time;
	float			frame_duration;
	int				enemy_hp;
	int				is_dead;
	int				id;
}					t_enemy;

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
	size_t			doors_counter;
	size_t			enemies_counter;
	t_door			*doors;
	t_enemy			*enemies;
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
	int				hp;
	float			dir_x;
	float			dir_y;
	float			plane_x;
	float			plane_y;
}					t_player;

typedef struct s_texture
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				frames_count;
	int				current_frame;
	int				width;
	int				height;
	char			*path;
	char			**frames;
	char			**frames_addr;
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
	int				is_shooting;
	int				shoot_ac;
	t_player		player;
	t_texture		north_texture;
	t_texture		south_texture;
	t_texture		east_texture;
	t_texture		west_texture;
	t_texture		door;
	t_texture		weapon_idle;
	t_texture		weapon_shoot;
	t_texture		enemy;
	t_texture		crosshair;
	t_texture		lose_img;
	t_texture		floor_texture;
	t_texture		ceiling_texture;
	float			*zbuffer;
	t_fight			fight;
	int				game_over;
}					t_game;

typedef struct s_trig_tables
{
	double			*sin_table;
	double			*cos_table;
}					t_trig_tables;

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
	int				is_door;
}					t_raycast;

typedef struct s_square
{
	int				x;
	int				y;
	int				size;
	int				color;
}					t_square;

typedef struct s_minimap_data
{
	int				block_size;
	int				minimap_tiles;
	float			player_tile_x;
	float			player_tile_y;
	float			start_x;
	float			start_y;
}					t_minimap_data;

typedef struct s_ctrl
{
	t_map			map;
	t_game			*game;
	t_gnl			gnl;
	t_texture		texture;
	t_anim			anim;
	t_trig_tables	*trig_tables;
}					t_ctrl;

/* ************************************************************************** */
/*              				game_cleanup.c                                */
/* ************************************************************************** */

void				free_map(char **map, size_t rows);
void				free_single_texture(t_game *game);
void				free_framed_texture(t_game *game, t_texture *texture);
void				game_cleanup(t_ctrl *ctrl);
void				clean_exit(char *str, t_ctrl *ctrl);
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

char				*ft_strjoin_three(const char *s1, const char *s2,
						const char *s3);
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
void				assign_enemy(t_enemy *enemy, int y, int x, int id);

/* ************************************************************************** */
/*              					read_map.c                                */
/* ************************************************************************** */

char				*read_map(char *filename, t_ctrl *ctrl);

/* ************************************************************************** */
/*									utils.c										*/
/* ************************************************************************** */

int					ft_strcmp(const char *s1, const char *s2);
char				*trim_trailing_whitespace(char *str);
int					ft_isspace(int c);
void				create_frame_paths(t_texture *tex, char *base_path,
						t_ctrl *ctrl);

/* ************************************************************************** */
/*									player.c									*/
/* ************************************************************************** */

int					key_release(int keycode, t_ctrl *ctrl);
int					key_press(int keycode, t_ctrl *ctrl);
bool				in_map_bounds(float x, float y, t_map *map);

/* ************************************************************************** */
/*								player_movement.c  								*/
/* ************************************************************************** */

void				move_player(t_ctrl *ctrl, double delta_time);

/* ************************************************************************** */
/*									parse_map.c  								*/
/* ************************************************************************** */

void				parse_map(char *filename, t_ctrl *ctrl);

/* ************************************************************************** */
/*										gnl.c  									*/
/* ************************************************************************** */

char				*gnl(int fd, t_ctrl *ctrl);

/* ************************************************************************** */
/*								error_checks.c  								*/
/* ************************************************************************** */

void				check_args(int argc, char *argv[], t_ctrl *ctrl);
void				check_valid_characters(t_ctrl *ctrl);
void				check_map_closed(t_ctrl *ctrl);

/* ************************************************************************** */
/*									flood_fill.c  								*/
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
int					load_texture(t_game *game, t_texture *texture,
						char **frames);
void				init_texture(t_texture *texture, int max_frames);

/* ************************************************************************** */
/*              					init_hooks.c                              */
/* ************************************************************************** */

void				init_hooks(t_ctrl *ctrl);
void				init_game_window(t_ctrl *ctrl);

// bonus
/* ************************************************************************** */
/*              						anim.c   								*/
/* ************************************************************************** */

void				select_frame(t_ctrl *ctrl);

int					mouse_move(int x, int y, t_ctrl *ctrl);
// void				update_doors(t_door *door, t_map *map);
void				update_doors(t_door *door, t_ctrl *ctrl, double delta_time);
t_door				*get_door(int grid_x, int grid_y, t_map *map);
void				door_open(int grid_x, int grid_y, t_map *map);

void				draw_minimap(t_map *map, t_game *game);
int					space_press(int keycode, t_ctrl *ctrl);
void				door_state(t_ctrl *ctrl);
t_trig_tables		*init_trig_tables(void);
void				free_trig_tables(t_trig_tables *tables);
void				draw_hp_bar(t_game *game, double delta_time);
int					check_enemy_visibility(t_enemy *enemy, t_ctrl *ctrl);
// void				enemy_attack(t_ctrl *ctrl);
void				enemy_attack(t_ctrl *ctrl, t_enemy *enemy);
float				compute_distance(float x1, float y1, float x2, float y2);
void				draw_skybox(t_game *gm);
void				shoot(t_ctrl *ctrl);
t_enemy				*get_enemy(size_t x, size_t y, t_map *map);
float				calculate_distance(float x1, float y1, float x2, float y2);

#endif