/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:59 by stefan            #+#    #+#             */
/*   Updated: 2025/01/20 13:40:29 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	hook_esc(t_ctrl *ctrl)
{
	game_cleanup(ctrl);
	exit(0);
}

// Initialize player with default values
void	init_player(t_player *player, float start_x, float start_y,
		float start_angle)
{
	player->x = start_x;
	player->y = start_y;
	player->angle = start_angle;
	player->key_up = false;
	player->key_down = false;
	player->key_right = false;
	player->key_left = false;
	player->left_rotate = false;
	player->right_rotate = false;
}

// Handle key press events
int	key_press(int keycode, t_ctrl *ctrl)
{
	t_game		*game;
	t_player	*player;

	game = ctrl->game;
	player = &game->player;
	if (keycode == KEY_W)
		player->key_up = true;
	if (keycode == KEY_S)
		player->key_down = true;
	if (keycode == KEY_A)
		player->key_left = true;
	if (keycode == KEY_D)
		player->key_right = true;
	if (keycode == KEY_LEFT)
		player->left_rotate = true;
	if (keycode == KEY_RIGHT)
		player->right_rotate = true;
	if (keycode == KEY_ESC)
		hook_esc(ctrl);
	return (0);
}

// Handle key release events
int	key_release(int keycode, t_ctrl *ctrl)
{
	t_game		*game;
	t_player	*player;

	game = ctrl->game;
	player = &game->player;
	if (keycode == KEY_W)
		player->key_up = false;
	if (keycode == KEY_S)
		player->key_down = false;
	if (keycode == KEY_A)
		player->key_left = false;
	if (keycode == KEY_D)
		player->key_right = false;
	if (keycode == KEY_LEFT)
		player->left_rotate = false;
	if (keycode == KEY_RIGHT)
		player->right_rotate = false;
	return (0);
}

// Check if coordinates are inside the map bounds
static bool	in_map_bounds(float x, float y, t_map *map)
{
	int	grid_x;
	int	grid_y;

	grid_x = (int)(x / TILE_SIZE);
	grid_y = (int)(y / TILE_SIZE);
	if (grid_x < 0 || grid_x >= (int)map->columns || grid_y < 0
		|| grid_y >= (int)map->rows)
	{
		return (false);
	}
	return (true);
}

// Check if a given position is valid (collision detection)
int	is_valid_position(float x, float y, t_map *map)
{
	int		grid_x;
	int		grid_y;
	char	tile;

	if (!in_map_bounds(x, y, map))
		return (0);
	grid_x = (int)(x / TILE_SIZE);
	grid_y = (int)(y / TILE_SIZE);
	tile = map->full_map[grid_y][grid_x];
	return (tile == '0' || tile == 'W');
}

void	update_player_angle(t_player *player, double delta_time)
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

void	apply_movement(float *new_x, float *new_y, t_player *player,
		float move_speed, float cos_angle, float sin_angle)
{
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

void	update_player_position(t_player *player, float move_speed, t_map *map,
		float cos_angle, float sin_angle)
{
	float	new_x;
	float	new_y;

	new_x = player->x;
	new_y = player->y;
	apply_movement(&new_x, &new_y, player, move_speed, cos_angle, sin_angle);
	if (is_valid_position(new_x, new_y, map))
	{
		player->x = new_x;
		player->y = new_y;
	}
}

void	move_player(t_player *player, double delta_time, t_map *map)
{
	float	move_speed;
	float	cos_angle;
	float	sin_angle;

	update_player_angle(player, delta_time);
	move_speed = 100.0f * delta_time;
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	update_player_position(player, move_speed, map, cos_angle, sin_angle);
}
