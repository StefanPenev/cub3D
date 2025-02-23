/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_attack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:05:08 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/23 13:10:32 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Reduces the enemy's HP by a fixed amount (PLAYER_DAMAGE).
// If HP reaches zero or below, marks the enemy as dead,
// updates the fight status,
// decreases the count of alive enemies, and checks for player victory.
// Prints appropriate messages when an enemy dies or all enemies are defeated.

static void	enemy_take_damage(t_ctrl *ctrl, t_enemy *enemy)
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

static t_enemy	*find_enemy_by_position(t_ctrl *ctrl, int x, int y)
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
