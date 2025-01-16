/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/15 16:57:57 by stefan           ###   ########.fr       */
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
	size_t	rows;
	size_t	columns;
	size_t players_count;
	t_pos	*players_positions;
	// size_t	n_position;
	// size_t	s_position;
	// size_t	e_position;
	// size_t	w_position;
}			t_map;

typedef struct s_ctrl
{
	t_map	map;
}			t_ctrl;

typedef struct s_game
{
	void	*mlx;
	void	*win;
}			t_game;

#endif