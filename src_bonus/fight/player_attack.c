/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_attack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:05:08 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/12 14:30:57 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	enemy_take_damage(t_enemy *enemy)
{
	enemy->enemy_hp -= PLAYER_DAMAGE;
	if (enemy->enemy_hp <= 0)
	{
		enemy->enemy_hp = 0;
		enemy->is_dead = 1;
		printf(COLOR_GREEN "Enemy is dead!\n" COLOR_RESET);
	}
}

int	is_enemy_hit(t_game *game, t_enemy *enemy, float ray_dir_x, float ray_dir_y)
{
	float	enemy_vec_x;
	float	enemy_vec_y;
	float	dot_product;
	float	enemy_distance;
	float	angle_diff;

	enemy_vec_x = enemy->x - game->player.x;
	enemy_vec_y = enemy->y - game->player.y;
	dot_product = (enemy_vec_x * ray_dir_x) + (enemy_vec_y * ray_dir_y);
	if (dot_product < 0)
		return (0);
	enemy_distance = sqrt(enemy_vec_x * enemy_vec_x + enemy_vec_y
			* enemy_vec_y);
	angle_diff = acos(dot_product / enemy_distance);
	return (angle_diff < 0.1);
}

void	shoot(t_ctrl *ctrl)
{
	float	ray_dir_x;
	float	ray_dir_y;
	t_enemy	*closest_enemy;
	t_enemy	*enemy;
	float	enemy_distance;
	float	ray_angle;
	float	closest_distance;
	int		i;

	i = 0;
	ray_angle = ctrl->game->player.angle;
	ray_dir_x = cos(ray_angle);
	ray_dir_y = sin(ray_angle);
	closest_enemy = NULL;
	closest_distance = 9999999;
	while (i < ctrl->map.enemies_counter)
	{
		enemy = &ctrl->map.enemies[i];
		if (is_enemy_hit(ctrl->game, enemy, ray_dir_x, ray_dir_y))
		{
			enemy_distance = calculate_distance(ctrl->game->player.x,
					ctrl->game->player.y, enemy->x, enemy->y);
			if (enemy_distance < closest_distance)
			{
				closest_distance = enemy_distance;
				closest_enemy = enemy;
			}
		}
		i++;
	}
	if (closest_enemy)
	{
		enemy_take_damage(closest_enemy);
		printf(COLOR_GREEN "Hit enemy!\n" COLOR_RESET);
	}
	else
	{
		printf(COLOR_RED "Miss!\n" COLOR_RESET);
	}
}
