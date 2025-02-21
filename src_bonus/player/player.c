/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:59 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 18:10:16 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	game_activities(int keycode, t_ctrl *ctrl)
{
	if (keycode == SPACE)
		space_press(keycode, ctrl);
	if (keycode == KEY_E)
		door_state(ctrl);
	if (keycode == KEY_H)
		ctrl->show_controls = !ctrl->show_controls;
	return (0);
}

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
	else
		game_activities(keycode, ctrl);
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

// Renders the player's HP bar on the screen.
// Calculates the width of the HP portion based on the current HP percentage.
// Handles damage if the player is attacked (get_hit).
// Restores HP when not in combat (restore_hp).
// Calls functions to draw the background (draw_bar_background),
// the red HP portion (draw_red_bar), and the frame (draw_bar_frame).

void	draw_hp_bar(t_game *game, double delta_time)
{
	int	hp_width;
	int	y;
	int	width;

	y = HEIGHT - 30;
	width = WIDTH / 4;
	if (game->fight.lose_flag || game->fight.win_flag)
		return ;
	hp_width = (width * game->player.hp) / 100;
	if (game->fight.fight_started && game->fight.enemy_shoot)
		get_hit(game);
	if (!game->fight.fight_started && game->player.hp < PLAYER_HP)
		restore_hp(game, delta_time);
	draw_bar_background(width, y, game);
	draw_red_bar(hp_width, y, game);
	draw_bar_frame(width, y, game);
}
