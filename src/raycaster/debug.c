/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:22:45 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:20:30 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * draw_map_debug - Draws the map in debug mode, coloring tiles.
 * @game: The game structure.
 * @map: The map structure.
 * @block_size: The size of each map block.
 */
static void	draw_map_debug(t_game *game, t_map *map, int block_size)
{
	size_t	y;
	size_t	x;
	int		color;

	y = 0;
	while (y < map->rows)
	{
		x = 0;
		while (x < map->columns)
		{
			if (map->full_map[y][x] == '1')
				color = 0xAAAAAA;
			else
				color = 0x333333;
			draw_square(init_square(x * block_size, y * block_size, block_size,
					color), game);
			x++;
		}
		y++;
	}
}

/**
 * draw_player_debug - Draws the player's position in debug mode.
 * @game: The game structure.
 * @block_size: The size of each block for the player's representation.
 */
static void	draw_player_debug(t_game *game, int block_size)
{
	int	player_x;
	int	player_y;

	player_x = (game->player.x / TILE_SIZE) * block_size;
	player_y = (game->player.y / TILE_SIZE) * block_size;
	draw_square(init_square(player_x - block_size / 8,
			player_y - block_size / 8, block_size / 4, 0x00FF00), game);
}

/**
 * draw_hit_line - Draws a line representing the raycast hit position.
 * @game: The game structure.
 * @rc: The raycast debug data.
 * @block_size: The size of each block in the map.
 */
static void	draw_hit_line(t_game *game, t_raycast_debug rc, int block_size)
{
	float	hit_pos_x;
	float	hit_pos_y;
	int		hit_x;
	int		hit_y;

	hit_pos_x = game->player.x + rc.ray_dir_x * rc.distance * TILE_SIZE;
	hit_pos_y = game->player.y + rc.ray_dir_y * rc.distance * TILE_SIZE;
	hit_x = (hit_pos_x / TILE_SIZE) * block_size;
	hit_y = (hit_pos_y / TILE_SIZE) * block_size;
	draw_line_coords(block_size, hit_x, hit_y, game);
}

/**
 * cast_rays_debug - Casts rays and draws the hit lines in debug mode.
 * @ctrl: The control structure.
 * @block_size: The size of each map block.
 */
static void	cast_rays_debug(t_ctrl *ctrl, int block_size)
{
	t_raycast_debug	rc;
	float			fov;
	float			angle_step;
	float			start_angle;
	int				i;

	fov = M_PI / 3.0f;
	angle_step = fov / WIDTH;
	start_angle = ctrl->game->player.angle - (fov / 2.0f);
	i = 0;
	while (i < WIDTH)
	{
		rc.ray_angle = start_angle + i * angle_step;
		normalize_angle(&rc.ray_angle);
		init_raycast_data(&rc, ctrl->game);
		while (!rc.hit && rc.grid_x < ctrl->map.columns
			&& rc.grid_y < ctrl->map.rows)
			ray_step_loop(&rc, &ctrl->map);
		if (rc.hit)
			draw_hit_line(ctrl->game, rc, block_size);
		i += 10;
	}
}

/**
 * draw_debug - Draws all the debug information including the map, player,
 * and rays.
 * @ctrl: The control structure.
 */
void	draw_debug(t_ctrl *ctrl)
{
	t_game	*game;
	t_map	*map;
	int		bw;
	int		bh;
	int		block_size;

	map = &ctrl->map;
	game = ctrl->game;
	bh = HEIGHT / map->rows;
	bw = WIDTH / map->columns;
	if (bw < bh)
		block_size = bw;
	else
		block_size = bh;
	draw_map_debug(game, map, block_size);
	draw_player_debug(game, block_size);
	cast_rays_debug(ctrl, block_size);
}
