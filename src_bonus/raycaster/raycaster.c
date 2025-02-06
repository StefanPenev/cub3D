/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:51 by stefan            #+#    #+#             */
/*   Updated: 2025/02/05 22:08:21 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	initialize_raycast(t_raycast *rc, t_player *pl, float angle)
{
	rc->ray_angle = angle;
	normalize_angle(&rc->ray_angle);
	rc->ray_dir_x = cosf(rc->ray_angle);
	rc->ray_dir_y = sinf(rc->ray_angle);
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

void	setup_steps(t_raycast *rc, t_player *pl)
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
// Adjusts the texture mapping for doors by applying an offset based on the door's state.
void	apply_door_offset(t_raycast *rc, t_map *map)
{
	t_door	*door;
	double	scale;
	double	old_wall_x;
	double	max_disp;
	double	displacement;

	scale = 2;    // Scaling factor for the displacement calculation
	max_disp = 1; // Maximum allowed displacement
	door = get_door(rc->map_x, rc->map_y, map);
	if (!door || door->state == DOOR_CLOSED) // Skip if no door or it's closed
		return ;
	old_wall_x = rc->wall_x;
	displacement = scale * (door->offset / (double)TILE_SIZE);
	if (displacement > max_disp)
		// Ensure displacement does not exceed the max limit
		displacement = max_disp;
	// Adjust wall_x based on door orientation
	if (door->orientation == VERTICAL)
		rc->wall_x = old_wall_x + displacement;
	else if (door->orientation == HORIZONTAL)
		rc->wall_x = old_wall_x - displacement;
	// Clamp wall_x between 0 and 1
	if (rc->wall_x < 0)
		rc->wall_x = 0;
	if (rc->wall_x > 1)
		rc->wall_x = 1;
	// Calculate texture X coordinate
	rc->tex_x = (int)(rc->wall_x * TEX_WIDTH);
	if (rc->tex_x >= TEX_WIDTH) // Ensure tex_x remains within valid range
		rc->tex_x = TEX_WIDTH - 1;
}

// Performs raycasting to detect walls or doors along a ray's path.
void	raycast_wall_hit(t_raycast *rc, t_map *map)
{
	while (!rc->hit) // Loop until a wall or door is hit
	{
		// Move along the shortest path (X or Y direction)
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side = 0; // Vertical wall hit
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side = 1; // Horizontal wall hit
		}
		// Break if outside map boundaries
		if (rc->map_x < 0 || (size_t)rc->map_x >= map->columns || rc->map_y < 0
			|| (size_t)rc->map_y >= map->rows)
			break ;
		// Check if a wall is hit
		if (touch(rc->map_x, rc->map_y, map))
			rc->hit = 1;
		// Check if a door is hit
		else if (map->full_map[rc->map_y][rc->map_x] == DOOR)
		{
			// If a door is hit, mark it and stop raycasting
			rc->hit = 1;
			rc->is_door = 1;
			// apply_door_offset(&rc, &map);
				// Uncomment to apply door offset logic
		}
	}
}

// Draws a door by applying texture mapping.
void	draw_door(t_ctrl *ctrl, t_raycast *rc, int col)
{
	rc->y = rc->draw_start;
	while (rc->y < rc->draw_end)
		// Loop through pixels from draw_start to draw_end
	{
		rc->tex_y = (int)rc->tex_pos & (TEX_HEIGHT - 1);
			// Get texture Y coordinate
		rc->tex_pos += rc->step;                        
			// Move to next texture position
		// Fetch the door's texture color
		rc->color = get_texture_color(&ctrl->game->door, rc->tex_x, rc->tex_y);
		// Draw the pixel on the screen
		put_pixel(col, rc->y, rc->color, ctrl->game);
		rc->y++;
	}
}

void	draw_line(t_player *player, t_ctrl *ctrl, float ray_angle,
		int screen_column)
{
	t_raycast	rc;

	initialize_raycast(&rc, player, ray_angle);
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
		apply_door_offset(&rc, &ctrl->map);
		draw_door(ctrl, &rc, screen_column);
	}
	else
	{
		choose_texture(&rc, ctrl);
		draw_wall(ctrl->game, &rc, screen_column);
	}
	draw_ceil_floor(ctrl->game, &rc, screen_column);
}

void	handle_rays(t_ctrl *ctrl, float start_angle, float angle_step)
{
	int i;
	float ray_angle;

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
