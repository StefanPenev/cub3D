/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:51 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 12:03:41 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * initialize_raycast - Initializes the raycasting structure for a given ray.
 * @rc: Pointer to the raycasting structure to initialize.
 * @pl: Pointer to the player structure containing player position.
 * @ctrl: Pointer to the control structure, providing access to trig tables.
 * @angle: The angle at which the ray is being cast.
 *
 * This function sets up the initial values for raycasting, including the ray's
 * direction, map position, and delta distances used for the DDA algorithm. It 
 * also utilizes pre-calculated trigonometric tables for optimization.
 */
static void	initialize_raycast(t_raycast *rc, t_player *pl, t_ctrl *ctrl,
	float angle)
{
	int	index;

	rc->ray_angle = angle;
	normalize_angle(&rc->ray_angle);
	index = get_table_index(rc->ray_angle);
	rc->ray_dir_x = ctrl->trig_tables->cos_table[index];
	rc->ray_dir_y = ctrl->trig_tables->sin_table[index];
	rc->map_x = (int)(pl->x / TILE_SIZE);
	rc->map_y = (int)(pl->y / TILE_SIZE);
	if (rc->ray_dir_x == 0.0f)
		rc->delta_dist_x = 1e30f;
	else
		rc->delta_dist_x = fabsf(1.0f / rc->ray_dir_x);
	if (rc->ray_dir_y == 0.0f)
		rc->delta_dist_y = 1e30f;
	else
		rc->delta_dist_y = fabsf(1.0f / rc->ray_dir_y);
	rc->hit = 0;
	rc->side = 0;
	rc->real_dist = 1e30f;
	rc->perp_dist = 1e30f;
	rc->is_door = 0;
}

/**
 * setup_steps - Determines the step direction and initial side distances.
 *  @rc: Pointer to the raycasting structure containing ray information.
 *  @pl: Pointer to the player structure containing player position.
 *
 * This function calculates:
 * - Step direction (`step_x` and `step_y`), which determines whether the ray
 * 	 moves
 *   left/right or up/down in the grid.
 * - The initial side distances (`side_dist_x` and `side_dist_y`), which
 *   define how
 *   far the ray is from the next grid boundary in both X and Y directions.
 */
static void	setup_steps(t_raycast *rc, t_player *pl)
{
	if (rc->ray_dir_x < 0.0f)
	{
		rc->step_x = -1;
		rc->side_dist_x = (pl->x / TILE_SIZE - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0f - pl->x / TILE_SIZE)
			* rc->delta_dist_x;
	}
	if (rc->ray_dir_y < 0.0f)
	{
		rc->step_y = -1;
		rc->side_dist_y = (pl->y / TILE_SIZE - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0f - pl->y / TILE_SIZE)
			* rc->delta_dist_y;
	}
}

/**
 * raycast_wall_hit - Performs the Digital Differential Analyzer (DDA) algorithm.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *  @map: Pointer to the map structure containing the game world.
 *
 * This function executes the raycasting loop to determine where the ray collides
 * with a wall. It continuously steps through the grid until a wall is hit, 
 * updating the current map position (`map_x`, `map_y`) and tracking which side
 * (horizontal or vertical) the collision occurs on.
 */
static void	raycast_wall_hit(t_raycast *rc, t_map *map)
{
	while (!rc->hit)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side = 1;
		}
		if (rc->map_x < 0 || (size_t)rc->map_x >= map->columns || rc->map_y < 0
			|| (size_t)rc->map_y >= map->rows)
			break ;
		if (touch(rc->map_x, rc->map_y, map))
			rc->hit = 1;
		else if (map->full_map[rc->map_y][rc->map_x] == DOOR)
		{
			rc->hit = 1;
			rc->is_door = 1;
		}
	}
}

/**
 * draw_line - Casts a ray to determine the wall hit and draws it on the screen.
 * @player: Pointer to the player structure containing the player's position.
 * @ctrl: Pointer to the control structure, providing access to game settings.
 * @ray_angle: The angle at which the ray is cast.
 * @screen_column: The screen column where the wall slice will be drawn.
 *
 * This function handles raycasting for a single column of the screen, 
 * performing necessary calculations, selecting textures, and drawing the 
 * wall slice, floor, and potentially a door.
 */
static void	draw_line(t_player *player, t_ctrl *ctrl, float ray_angle,
		int screen_column)
{
	t_raycast	rc;

	initialize_raycast(&rc, player, ctrl, ray_angle);
	setup_steps(&rc, player);
	raycast_wall_hit(&rc, &ctrl->map);
	compute_wall_dimensions(&rc, player);
	compute_wall_x(&rc);
	if (rc.is_door)
	{
		rc.selected_texture = ctrl->game->door.img;
		rc.step = 1.0f * TEX_HEIGHT / rc.wall_height;
		rc.tex_pos = (rc.draw_start - HEIGHT / 2 + rc.wall_height / 2)
			* rc.step;
		draw_door(ctrl, &rc, screen_column);
	}
	else
	{
		choose_texture(&rc, ctrl);
		draw_wall(ctrl->game, &rc, screen_column);
	}
	draw_floor(ctrl->game, &rc, screen_column);
}

/**
 * handle_rays - Casts rays across the screen to render the 3D view.
 * 	@ctrl: Pointer to the game control structure.
 * 	@start_angle: The initial angle for the first ray.
 * 	@angle_step: The angle increment between consecutive rays.
 *
 * This function iterates through each vertical column of the screen, casting a
 * ray at an appropriate angle and rendering the scene accordingly.
 * It ensures the angle remains within the range [0, 2π].
 */
void	handle_rays(t_ctrl *ctrl, float start_angle, float angle_step)
{
	int		i;
	float	ray_angle;

	i = 0;
	while (i < WIDTH)
	{
		ray_angle = start_angle + (i * angle_step);
		if (ray_angle < 0)
			ray_angle += 2.0f * M_PI;
		else if (ray_angle >= 2.0f * M_PI)
			ray_angle -= 2.0f * M_PI;
		draw_line(&ctrl->game->player, ctrl, ray_angle, i);
		i++;
	}
}
