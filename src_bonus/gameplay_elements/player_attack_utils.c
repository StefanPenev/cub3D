/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_attack_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:18:02 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/19 15:27:46 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Calculates the delta distances for the ray direction along the X and Y axes.
// Delta distances represent how much distance the
// ray must travel along each axis to move from one grid line to the next.

static void	calculate_deltas(t_raycast *ray)
{
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
}

// Checks if the ray's current position is within the map boundaries.
// Returns 1 if the position is valid, or 0 if the ray has left the map.

int	is_ray_in_bounds(t_ctrl *ctrl, t_raycast *ray)
{
	return (ray->map_x >= 0 && ray->map_x < (int)ctrl->map.columns
		&& ray->map_y >= 0 && ray->map_y < (int)ctrl->map.rows);
}

// Advances the ray step-by-step along the map grid.
// Determines whether the ray should move along the X or Y direction
// based on which delta distance is smaller.
// Updates the map coordinates and recalculates delta distances.

static void	advance_ray(t_raycast *ray)
{
	if (ray->delta_dist_x < ray->delta_dist_y)
	{
		if (ray->ray_dir_x > 0)
			ray->map_x++;
		else
			ray->map_x--;
		ray->delta_dist_x += fabs(1 / ray->ray_dir_x);
	}
	else
	{
		if (ray->ray_dir_y > 0)
			ray->map_y++;
		else
			ray->map_y--;
		ray->delta_dist_y += fabs(1 / ray->ray_dir_y);
	}
}

// Checks whether the enemy is within a valid shooting range (≤ 2 tiles).
// If the enemy is too far, prints a "Miss" message and returns 0.
// Returns 1 if the enemy is within range.

int	enemy_in_range(t_ctrl *ctrl, t_enemy *enemy)
{
	double	distance;

	distance = compute_distance(ctrl->game->player.x, ctrl->game->player.y,
			enemy->x, enemy->y) / TILE_SIZE;
	if (distance > 2.0)
	{
		printf(COLOR_RED "Miss! Enemy is too far (%.2f tiles)\n" COLOR_RESET,
			distance);
		return (0);
	}
	return (1);
}

// Casts a ray from the player's position at a specified angle.
// Continues advancing the ray until it hits a wall ('1') or an enemy ('M'),
// or leaves the map boundaries. Sets ray->hit to 1 when a hit occurs.

void	cast_ray(t_ctrl *ctrl, t_raycast *ray, float angle)
{
	char	hit_object;

	ray->ray_dir_x = cos(angle);
	ray->ray_dir_y = sin(angle);
	ray->map_x = (int)(ctrl->game->player.x / TILE_SIZE);
	ray->map_y = (int)(ctrl->game->player.y / TILE_SIZE);
	ray->hit = 0;
	calculate_deltas(ray);
	while (!ray->hit)
	{
		if (!is_ray_in_bounds(ctrl, ray))
		{
			ray->hit = 1;
			return ;
		}
		advance_ray(ray);
		hit_object = ctrl->map.full_map[ray->map_y][ray->map_x];
		if (hit_object == '1' || hit_object == 'M')
			ray->hit = 1;
	}
}
