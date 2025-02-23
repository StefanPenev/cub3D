/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:10:00 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:13:05 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * is_valid_position - Checks if a position is valid on the map.
 * @x: The x-coordinate to check.
 * @y: The y-coordinate to check.
 * @map: The map structure.
 * 
 * Returns 1 if the position is valid (empty or walkable tile), otherwise 0.
 */
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
	return (tile == '0' || tile == 'W' || tile == 'N'
		|| tile == 'S' || tile == 'E');
}

/**
 * update_player_angle - Updates the player's angle based on rotation input.
 * @player: The player object.
 * @delta_time: The time since the last frame.
 * 
 * Adjusts the player's angle based on left or right rotation input.
 */
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

/**
 * apply_movement - Updates the player's position based on movement input.
 * @new_x: Pointer to the new x-coordinate.
 * @new_y: Pointer to the new y-coordinate.
 * @player: The player object.
 * @move_speed: The speed at which the player moves.
 * 
 * Calculates the new player position based on movement keys (W, A, S, D).
 */
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

/**
 * update_player_position - Updates the player's position if it's valid.
 * @ctrl: The control structure containing game and map data.
 * @move_speed: The speed at which the player moves.
 * 
 * Calculates and updates the player's position if the new position is valid.
 */
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

/**
 * move_player - Moves the player based on input and delta time.
 * @ctrl: The control structure containing game and player data.
 * @delta_time: The time since the last frame.
 * 
 * Updates the player's angle and position based on input and time.
 */
void	move_player(t_ctrl *ctrl, double delta_time)
{
	float	move_speed;

	update_player_angle(&ctrl->game->player, delta_time);
	move_speed = 100.0f * delta_time;
	update_player_position(ctrl, move_speed);
}
