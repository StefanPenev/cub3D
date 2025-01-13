/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:11:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/13 15:55:56 by anilchen         ###   ########.fr       */
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

typedef struct s_map
{
	char	**full_map;
	size_t		rows;
	size_t		columns;
	// size_t		keys;
	// size_t		exit;
	// size_t		players;
}			t_map;

typedef struct s_ctrl
{
	t_map	map;
}			t_ctrl;

#endif