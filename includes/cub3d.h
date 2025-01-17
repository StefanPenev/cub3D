/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/17 09:48:33 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <mlx.h>
# include <stdio.h>
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

# define WIDTH 800
# define HEIGHT 600

typedef struct s_pos
{
	size_t	x;
	size_t	y;

}			t_pos;

typedef struct s_map
{
	char	**full_map;
	int		has_free_way;
	size_t	rows;
	size_t	columns;
	size_t	players_count;
	t_pos	*players_positions;
}			t_map;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_data;
	int		bpp;
	int		size_line;
	int		endian;
}			t_game;

typedef struct s_ctrl
{
	t_map	map;
	t_game	*game;
}			t_ctrl;

void		free_map(char **map, size_t rows);
void		game_cleanup(t_ctrl *ctrl);
void		clean_exit(char *str, t_ctrl *ctrl);
void		check_args(int argc, char *argv[], t_ctrl *ctrl);
void		check_valid_characters(t_ctrl *ctrl);
void		check_map_closed(t_ctrl *ctrl);
void		check_map_valid(t_ctrl *ctrl);
void		parse_map(char *filename, t_ctrl *ctrl);
void		find_biggest_line(t_ctrl *ctrl);
char		*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp);
int			hook_keypress(int keycode, t_ctrl *ctrl);

#endif