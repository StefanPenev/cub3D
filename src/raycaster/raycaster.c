/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:51 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 14:54:24 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	raycast_wall_hit(t_raycast *rc, t_map *map)
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
		if (rc->map_x < 0 || (size_t)rc->map_x >= map->columns
			|| rc->map_y < 0 || (size_t)rc->map_y >= map->rows)
			break ;
		if (touch(rc->map_x, rc->map_y, map))
			rc->hit = 1;
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
	choose_texture(&rc, ctrl->game);
	draw_wall(ctrl->game, &rc, screen_column);
	draw_ceil_floor(ctrl->game, &rc, screen_column);
}

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
