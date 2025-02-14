/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_attack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:16:47 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 15:05:51 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

int	check_enemy_visibility(t_enemy *enemy, t_ctrl *ctrl)
{
	double	cur_x;
	double	cur_y;
	double	enemy_px;
	double	enemy_py;
	double	player_px;
	double	player_py;
	double	dx;
	double	dy;
	double	distance;
	double	ray_angle;
	double	step;
	double	cur_dist;
	int		grid_x;
	int		grid_y;
	t_door	*door;

	door = NULL;
	enemy_px = enemy->x;
	enemy_py = enemy->y;
	player_px = ctrl->game->player.x;
	player_py = ctrl->game->player.y;
	dx = player_px - enemy_px;
	dy = player_py - enemy_py;
	distance = compute_distance(player_px, player_py, enemy_px, enemy_py);
	ray_angle = atan2(dy, dx);
	step = TILE_SIZE / 8.0;
	cur_dist = 0;
	while (cur_dist < distance)
	{
		cur_x = enemy_px + cur_dist * cos(ray_angle);
		cur_y = enemy_py + cur_dist * sin(ray_angle);
		grid_x = (int)(cur_x / TILE_SIZE);
		grid_y = (int)(cur_y / TILE_SIZE);
		if (ctrl->map.full_map[grid_y][grid_x] == '1')
			return (0);
		door = get_door(grid_x, grid_y, &ctrl->map);
		if (door != NULL && door->state == DOOR_CLOSED)
			return (0);
		cur_dist += step;
	}
	return (1);
}

void	enemy_attack(t_ctrl *ctrl, t_enemy *enemy)
{
	static int	frame_count;
	double		distance;

	// float		distance;
	if (enemy->is_dead)
		return ;
	distance = compute_distance(enemy->x, enemy->y, ctrl->game->player.x,
			ctrl->game->player.y) / TILE_SIZE;
	// distance = calculate_distance(enemy->x, enemy->y, ctrl->game->player.x,
	//		ctrl->game->player.y);
	if (distance > 2.0f)
	{
		return ;
	}
	frame_count++;
	if (frame_count < ctrl->game->fight.shoot_delay)
	{
		return ;
	}
	if (!ctrl->game->fight.lose_flag)
	{
		ctrl->game->fight.enemy_shoot = 1;
		printf(COLOR_RED "Enemy %d attacks!\n" COLOR_RESET, enemy->id);
		frame_count = 0;
	}
}

// void	enemy_attack(t_ctrl *ctrl)
// {
// 	static int	frame_count;

// 	frame_count++;
// 	if (frame_count < ctrl->game->fight.shoot_delay)
// 		return ;
// 	for (size_t i = 0; i < ctrl->map.enemies_counter; i++)
// 	{
// 		if (ctrl->map.enemies[i].is_dead)
// 			continue ;
// 		if (!ctrl->game->fight.lose_flag)
// 		{
// 			ctrl->game->fight.enemy_shoot = 1;
// 			printf(COLOR_RED "Enemy %zu attacks!\n" COLOR_RESET, i);
// 			frame_count = 0;
// 		}
// 	}
// }
