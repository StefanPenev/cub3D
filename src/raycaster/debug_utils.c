/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:45:05 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:22:46 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_line_params - Initializes the parameters for drawing a line.
 * @start: The starting coordinates of the line.
 * @x_end: The x-coordinate of the line's endpoint.
 * @y_end: The y-coordinate of the line's endpoint.
 * @lp: The structure holding the line parameters.
 */
void	init_line_params(int start[], int x_end, int y_end, t_line_params *lp)
{
	int	x_start;
	int	y_start;

	x_start = start[0];
	y_start = start[1];
	lp->delta_x = abs(x_end - x_start);
	lp->delta_y = abs(y_end - y_start);
	if (x_start < x_end)
		lp->step_x = 1;
	else
		lp->step_x = -1;
	if (y_start < y_end)
		lp->step_y = 1;
	else
		lp->step_y = -1;
	lp->error_val = lp->delta_x - lp->delta_y;
}

/**
 * draw_line_coords - Draws a line on the map using given coordinates.
 * @block_size: The size of each block in the grid.
 * @x_end: The x-coordinate of the line's endpoint.
 * @y_end: The y-coordinate of the line's endpoint.
 * @game: The game structure for access to the drawing functions.
 */
void	draw_line_coords(int block_size, int x_end, int y_end, t_game *game)
{
	t_line_params	lp;
	int				start[2];

	start[0] = (game->player.x / TILE_SIZE) * block_size;
	start[1] = (game->player.y / TILE_SIZE) * block_size;
	init_line_params(start, x_end, y_end, &lp);
	while (1)
	{
		draw_square(init_square(start[0], start[1], 1, 0x00FF00), game);
		if (start[0] == x_end && start[1] == y_end)
			break ;
		lp.error_val2 = 2 * lp.error_val;
		if (lp.error_val2 > -lp.delta_y)
		{
			lp.error_val -= lp.delta_y;
			start[0] += lp.step_x;
		}
		if (lp.error_val2 < lp.delta_x)
		{
			lp.error_val += lp.delta_x;
			start[1] += lp.step_y;
		}
	}
}

/**
 * ray_step_loop - Steps through the raycasting process until a hit is found.
 * @rc: The raycast debug structure holding ray data.
 * @map: The map structure with the grid to be checked.
 */
void	ray_step_loop(t_raycast_debug *rc, t_map *map)
{
	while (!rc->hit && rc->grid_x < map->columns && rc->grid_y < map->rows)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->grid_x += rc->step_x;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->grid_y += rc->step_y;
		}
		if (touch(rc->grid_x, rc->grid_y, map))
		{
			rc->hit = 1;
			if (rc->side_dist_x < rc->side_dist_y)
				rc->distance = rc->side_dist_x;
			else
				rc->distance = rc->side_dist_y;
		}
	}
}

/**
 * init_raycast_data_steps - Initializes the step calculations for raycasting.
 * @rc: The raycast debug structure holding ray data.
 * @game: The game structure for player information.
 */
static void	init_raycast_data_steps(t_raycast_debug *rc, t_game *game)
{
	float	px;
	float	py;

	px = game->player.x / TILE_SIZE;
	py = game->player.y / TILE_SIZE;
	if (rc->ray_dir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (px - rc->grid_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->grid_x + 1.0f - px) * rc->delta_dist_x;
	}
	if (rc->ray_dir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (py - rc->grid_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->grid_y + 1.0f - py) * rc->delta_dist_y;
	}
}

/**
 * init_raycast_data - Initializes the raycasting data for the current ray.
 * @rc: The raycast debug structure to be initialized.
 * @game: The game structure for access to player position.
 */
void	init_raycast_data(t_raycast_debug *rc, t_game *game)
{
	rc->hit = 0;
	rc->distance = 0.0f;
	rc->grid_x = (size_t)(game->player.x / TILE_SIZE);
	rc->grid_y = (size_t)(game->player.y / TILE_SIZE);
	rc->ray_dir_x = cosf(rc->ray_angle);
	rc->ray_dir_y = sinf(rc->ray_angle);
	if (rc->ray_dir_x != 0.0f)
		rc->delta_dist_x = fabsf(1.0f / rc->ray_dir_x);
	else
		rc->delta_dist_x = 1e30f;
	if (rc->ray_dir_y != 0.0f)
		rc->delta_dist_y = fabsf(1.0f / rc->ray_dir_y);
	else
		rc->delta_dist_y = 1e30f;
	init_raycast_data_steps(rc, game);
}
