/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:59 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 14:30:14 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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
