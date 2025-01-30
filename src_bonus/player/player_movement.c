/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:10:00 by stefan            #+#    #+#             */
/*   Updated: 2025/01/30 16:41:26 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	door_open(int grid_x, int grid_y, t_map *map)
{
	map->door_open = 1;
	if (map->full_map[grid_y][grid_x - 1] == DOOR)
		map->full_map[grid_y][grid_x - 1] = EMPTY;
	if (map->full_map[grid_y][grid_x + 1] == DOOR)
		map->full_map[grid_y][grid_x + 1] = EMPTY;
	if (map->full_map[grid_y - 1][grid_x] == DOOR)
		map->full_map[grid_y - 1][grid_x] = EMPTY;
	if (map->full_map[grid_y + 1][grid_x] == DOOR)
		map->full_map[grid_y + 1][grid_x] = EMPTY;
}

static int	is_valid_position(float x, float y, t_map *map)
{
	int		grid_x;
	int		grid_y;
	char	tile;

	if (!in_map_bounds(x, y, map))
		return (0);
	grid_x = (int)(x / TILE_SIZE);
	grid_y = (int)(y / TILE_SIZE);
	tile = map->full_map[grid_y][grid_x];
	if (map->full_map[grid_y + 1][grid_x] == DOOR || map->full_map[grid_y
		- 1][grid_x] == DOOR || map->full_map[grid_y][grid_x + 1] == DOOR
		|| map->full_map[grid_y][grid_x - 1] == DOOR)
	{
		door_open(grid_x, grid_y, map);
	}
	else
	{
		map->door_open = 0;
	}
	return (tile == '0' || tile == 'W' || tile == 'N' || tile == 'S'
		|| tile == 'E');
}

static void	update_player_angle(t_player *player, double delta_time)
{
	float	angle_speed;

	angle_speed = 1.5f * delta_time;
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * M_PI)
		player->angle -= 2 * M_PI;
	else if (player->angle < 0)
		player->angle += 2 * M_PI;
}

static void	apply_movement(float *new_x, float *new_y, t_player *player,
		float move_speed)
{
	float	cos_angle;
	float	sin_angle;

	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	if (player->key_up)
	{
		*new_x += cos_angle * move_speed;
		*new_y += sin_angle * move_speed;
	}
	if (player->key_down)
	{
		*new_x -= cos_angle * move_speed;
		*new_y -= sin_angle * move_speed;
	}
	if (player->key_left)
	{
		*new_x += sin_angle * move_speed;
		*new_y -= cos_angle * move_speed;
	}
	if (player->key_right)
	{
		*new_x -= sin_angle * move_speed;
		*new_y += cos_angle * move_speed;
	}
}

static void	update_player_position(t_ctrl *ctrl, float move_speed)
{
	float	new_x;
	float	new_y;

	new_x = ctrl->game->player.x;
	new_y = ctrl->game->player.y;
	apply_movement(&new_x, &new_y, &ctrl->game->player, move_speed);
	if (is_valid_position(new_x, new_y, &ctrl->map))
	{
		ctrl->game->player.x = new_x;
		ctrl->game->player.y = new_y;
	}
}

void	move_player(t_ctrl *ctrl, double delta_time)
{
	float	move_speed;

	update_player_angle(&ctrl->game->player, delta_time);
	move_speed = 100.0f * delta_time;
	update_player_position(ctrl, move_speed);
}
