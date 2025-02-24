/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_attack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:16:47 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/24 09:53:11 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Computes the angle and distance between the player and the enemy
// This function calculates :
// The angle(ray_angle) using atan2 based on player and enemy positions.-
// The Euclidean distance between the player and the enemy
// param enemy Pointer to the enemy whose visibility is being checked.
// param ctrl Pointer to the main controller structure containing player data
// param ray_angle Pointer to store the calculated angle between player
// and enemy
// param distance Pointer to store the calculated distance between player
// and enemy.

static void	compute_ray_properties(t_enemy *enemy, t_ctrl *ctrl,
		double *ray_angle, double *distance)
{
	double	dx;
	double	dy;

	dx = ctrl->game->player.x - enemy->x;
	dy = ctrl->game->player.y - enemy->y;
	*distance = compute_distance(ctrl->game->player.x, ctrl->game->player.y,
			enemy->x, enemy->y);
	*ray_angle = atan2(dy, dx);
}

// Checks if the line of sight at the given coordinates is clear.
//  This function verifies whether there are obstacles (walls or closed doors)
//   at a specific position in the map. It converts the world coordinates to
//   grid coordinates and checks the map data.
//  param ctrl Pointer to the main controller structure containing map data.
//  param cur_x The current x-coordinate being checked along the ray path.
//  param cur_y The current y-coordinate being checked along the ray path.
//  return (int Returns 1 if the path is clear); 0 if blocked by a wall or
// closed door.

static int	check_line_of_sight(t_ctrl *ctrl, double cur_x, double cur_y)
{
	t_door	*door;
	int		grid_x;
	int		grid_y;

	grid_x = (int)(cur_x / TILE_SIZE);
	grid_y = (int)(cur_y / TILE_SIZE);
	if (ctrl->map.full_map[grid_y][grid_x] == '1')
		return (0);
	door = get_door(grid_x, grid_y, &ctrl->map);
	if (door != NULL && door->state == DOOR_CLOSED)
		return (0);
	return (1);
}

// Checks if the path between the enemy and the player is clear.
// This function uses raycasting by incrementally checking points along
// the line connecting the enemy and the player. It stops if a wall or
// closed door is detected.
// param enemy Pointer to the enemy whose visibility is being checked.
// param ctrl Pointer to the main controller structure containing map data.
// param ray_angle Angle at which the ray is cast from the enemy to the player.
// param distance The total distance between the enemy and the player.
// return (int Returns 1 if the path is clear); 0 otherwise.

static int	is_path_clear(t_enemy *enemy, t_ctrl *ctrl, double ray_angle,
		double distance)
{
	double	step;
	double	cur_dist;
	double	cur_x;
	double	cur_y;
	int		index;

	cur_dist = 0;
	step = TILE_SIZE / 8.0;
	index = get_table_index(ray_angle);
	while (cur_dist < distance)
	{
		cur_x = enemy->x + cur_dist * ctrl->trig_tables->cos_table[index];
		cur_y = enemy->y + cur_dist * ctrl->trig_tables->sin_table[index];
		if (!check_line_of_sight(ctrl, cur_x, cur_y))
			return (0);
		cur_dist += step;
	}
	return (1);
}

// Determines if player is visible to the enemy.
// Combines ray property computation and path clearance checks
// to determine whether player can be seen by the enemy.
// Visibility is blocked by walls or closed doors.
// param enemy Pointer to the enemy being checked.
// param ctrl Pointer to the main controller structure.
// return (int Returns 1 if the enemy is visible); 0 otherwise.

int	check_enemy_visibility(t_enemy *enemy, t_ctrl *ctrl)
{
	double	ray_angle;
	double	distance;

	compute_ray_properties(enemy, ctrl, &ray_angle, &distance);
	return (is_path_clear(enemy, ctrl, ray_angle, distance));
}

// brief Simulates an enemy attack on the player.
// This function checks if:
// - The enemy is alive.
// - The player is within attacking range (distance ≤ 2 tiles).
// - The attack delay has passed since the last attack.
// If conditions are met, the enemy attacks, and a message is printed.
// param ctrl Pointer to the main controller structure containing fight data.
// param enemy Pointer to the enemy performing the attack.

void	enemy_attack(t_ctrl *ctrl, t_enemy *enemy)
{
	static int	frame_count;
	double		distance;

	if (enemy->is_dead)
		return ;
	distance = compute_distance(enemy->x, enemy->y, ctrl->game->player.x,
			ctrl->game->player.y) / TILE_SIZE;
	if (distance > 2.0f)
		return ;
	frame_count++;
	if (frame_count < ctrl->game->fight.shoot_delay)
		return ;
	if (!ctrl->game->fight.lose_flag)
	{
		ctrl->game->fight.enemy_shoot = 1;
		printf(COLOR_RED "Enemy %d attacks!\n" COLOR_RESET, enemy->id);
		frame_count = 0;
	}
}
