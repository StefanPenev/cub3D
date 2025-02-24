/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:59 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 14:15:45 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * key_press - Handles key press events for player movement and rotation.
 * @keycode: The key that was pressed.
 * @ctrl: The control structure containing game and player data.
 * 
 * Sets flags for player movement or rotation based on the key pressed. 
 * Closes the window if the Escape key is pressed.
 */
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
		close_window(ctrl);
	return (0);
}

/**
 * key_release - Handles key release events, stopping player movement or
 * rotation.
 * @keycode: The key that was released.
 * @ctrl: The control structure containing game and player data.
 * 
 * Resets flags for player movement or rotation based on the key released.
 */
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

/**
 * in_map_bounds - Checks if a position is within the map boundaries.
 * @x: The x-coordinate to check.
 * @y: The y-coordinate to check.
 * 	map: The map structure containing the map data.
 * 
 * Returns true if the position is within the map, false otherwise.
 */
bool	in_map_bounds(float x, float y, t_map *map)
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
