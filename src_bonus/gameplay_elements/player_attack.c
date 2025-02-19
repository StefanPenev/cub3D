/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_attack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:05:08 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/19 15:25:10 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Reduces the enemy's HP by a fixed amount (PLAYER_DAMAGE).
// If HP reaches zero or below, marks the enemy as dead,
// updates the fight status,
// decreases the count of alive enemies, and checks for player victory.
// Prints appropriate messages when an enemy dies or all enemies are defeated.

void	enemy_take_damage(t_ctrl *ctrl, t_enemy *enemy)
{
	if (!enemy)
		return ;
	enemy->enemy_hp -= PLAYER_DAMAGE;
	if (enemy->enemy_hp <= 0)
	{
		enemy->enemy_hp = 0;
		enemy->is_dead = 1;
		ctrl->game->fight.fight_started = 0;
		ctrl->game->fight.alive_enemies--;
		printf(COLOR_GREEN "Enemy %d is dead!\n" COLOR_RESET, enemy->id);
		if (ctrl->game->fight.alive_enemies <= 0)
		{
			ctrl->game->fight.win_flag = 1;
			printf(COLOR_GREEN "All enemies defeated! You win!\n" COLOR_RESET);
		}
	}
}

// Searches for an enemy at the given grid coordinates (x, y).
// Converts the enemy's world position to grid coordinates for comparison.
// Returns a pointer to the enemy if found; otherwise, returns NULL.

t_enemy	*find_enemy_by_position(t_ctrl *ctrl, int x, int y)
{
	size_t	i;
	int		enemy_map_x;
	int		enemy_map_y;

	i = 0;
	while (i < ctrl->map.enemies_counter)
	{
		enemy_map_x = (int)(ctrl->map.enemies[i].x / TILE_SIZE);
		enemy_map_y = (int)(ctrl->map.enemies[i].y / TILE_SIZE);
		if (enemy_map_x == x && enemy_map_y == y)
			return (&ctrl->map.enemies[i]);
		i++;
	}
	return (NULL);
}

// Handles the logic when a ray hits an enemy.
// Finds the enemy based on the ray's current map coordinates.
// Checks if the enemy is already dead or out of range before applying damage.
// If all enemies are defeated, stops further messages.

static void	handle_enemy_hit(t_ctrl *ctrl, t_raycast *ray)
{
	t_enemy	*enemy;

	enemy = find_enemy_by_position(ctrl, ray->map_x, ray->map_y);
	if (!enemy || enemy->is_dead)
		return ;
	if (!enemy_in_range(ctrl, enemy))
		return ;
	enemy_take_damage(ctrl, enemy);
	if (!ctrl->game->fight.win_flag)
		printf(COLOR_GREEN "Hit enemy!\n" COLOR_RESET);
}

// Handles the shooting process initiated by the player.
// Casts a ray in the direction the player is facing.
// Checks if the ray hits an enemy and applies damage if appropriate.
// Outputs "Hit enemy!" or "Miss!" depending on the outcome.

void	shoot(t_ctrl *ctrl)
{
	t_raycast	ray;

	cast_ray(ctrl, &ray, ctrl->game->player.angle);
	if (!is_ray_in_bounds(ctrl, &ray))
		return ;
	if (ray.hit && ctrl->map.full_map[ray.map_y][ray.map_x] == 'M')
		handle_enemy_hit(ctrl, &ray);
	else
		printf(COLOR_RED "Miss!\n" COLOR_RESET);
}

// void	cast_ray(t_ctrl *ctrl, t_raycast *ray, float start_x, float start_y,
// 		float angle)
// {
// 	int		tile_size;
// 	float	delta_dist_x;
// 	float	delta_dist_y;
// 	char	hit_object;

// 	tile_size = TILE_SIZE;
// 	ray->ray_dir_x = cos(angle);
// 	ray->ray_dir_y = sin(angle);
// 	ray->map_x = (int)(start_x / tile_size);
// 	ray->map_y = (int)(start_y / tile_size);
// 	ray->hit = 0;
// 	delta_dist_x = fabs(1 / ray->ray_dir_x);
// 	delta_dist_y = fabs(1 / ray->ray_dir_y);
// 	while (!ray->hit)
// 	{
// 		if (ray->map_x < 0 || ray->map_x >= (int)ctrl->map.columns
// 			|| ray->map_y < 0 || ray->map_y >= (int)ctrl->map.rows)
// 		{
// 			ray->hit = 1;
// 			return ;
// 		}
// 		if (delta_dist_x < delta_dist_y)
// 		{
// 			ray->map_x += (ray->ray_dir_x > 0) ? 1 : -1;
// 			delta_dist_x += fabs(1 / ray->ray_dir_x);
// 		}
// 		else
// 		{
// 			ray->map_y += (ray->ray_dir_y > 0) ? 1 : -1;
// 			delta_dist_y += fabs(1 / ray->ray_dir_y);
// 		}
// 		hit_object = ctrl->map.full_map[ray->map_y][ray->map_x];
// 		if (hit_object == '1' || hit_object == 'M')
// 			ray->hit = 1;
// 	}
// }

// void	shoot(t_ctrl *ctrl)
// {
// 	t_raycast	ray;
// 	t_enemy		*enemy;
// 	double		enemy_distance;
// 	float		cross_x;
// 	float		cross_y;

// 	cross_x = ctrl->game->player.x + cos(ctrl->game->player.angle) * 1.0;
// 	cross_y = ctrl->game->player.y + sin(ctrl->game->player.angle) * 1.0;
// 	cast_ray(ctrl, &ray, cross_x, cross_y, ctrl->game->player.angle);
// 	if (ray.map_x < 0 || ray.map_x >= (int)ctrl->map.columns || ray.map_y < 0
// 		|| ray.map_y >= (int)ctrl->map.rows)
// 		return ;
// 	if (ray.hit && ctrl->map.full_map[ray.map_y][ray.map_x] == 'M')
// 	{
// 		enemy = find_enemy_by_position(ctrl, ray.map_x, ray.map_y);
// 		if (!enemy)
// 			return ;
// 		if (enemy->is_dead)
// 			return ;
// 		enemy_distance = compute_distance(ctrl->game->player.x,
// 				ctrl->game->player.y, enemy->x, enemy->y) / TILE_SIZE;
// 		if (enemy_distance > 2.0)
// 		{
// 			printf(COLOR_RED "Miss! Enemy is too far)\n" COLOR_RESET);
// 			return ;
// 		}
// 		enemy_take_damage(ctrl, enemy);
// 		if (ctrl->game->fight.win_flag)
// 			return ;
// 		printf(COLOR_GREEN "Hit enemy!\n" COLOR_RESET);
// 	}
// 	else
// 		printf(COLOR_RED "Miss!\n" COLOR_RESET);
// }
