/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:10:00 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 11:05:46 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Checks if the player's new position is within valid map boundaries.
// Converts the player's floating-point coordinates into grid indices.
// Determines if the grid cell corresponds to a walkable tile
// Returns 1 if the position is valid for movement, otherwise returns 0.

static int	is_valid_position(float x, float y, t_map *map)
{
	size_t	grid_x;
	size_t	grid_y;
	char	tile;

	if (!in_map_bounds(x, y, map))
		return (0);
	grid_x = (size_t)(x / TILE_SIZE);
	grid_y = (size_t)(y / TILE_SIZE);
	tile = map->full_map[grid_y][grid_x];
	return (tile == '0' || tile == 'W' || tile == 'N' || tile == 'S'
		|| tile == 'E' || tile == 'C');
}

// Updates the player's facing angle based on rotation input.
// Adjusts the angle based on whether the player rotates left or right.
// Ensures the angle remains within the 0 to 2*PI
// range (wrap-around behavior).
// Uses delta_time to make the rotation frame-rate independent.

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

// Calculates the player's new potential position based on movement input.
// Uses trigonometric functions (cos and sin) to move in the direction
// of the player's angle. Supports forward, backward, left, and right
// movement relative to the player's orientation.  Modifies new_x and
// new_y pointers with the computed displacement based on move_speed.

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

// Attempts to move the player to a new position based on current
// movement input.
// Calls `apply_movement` to calculate the new position.
// Verifies the new position is valid using `is_valid_position`.
// Updates the player's position only if the movement does not
// collide with walls or invalid tiles.

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

// Handles the complete player movement logic per frame.
// First, updates the player's angle based on rotation input
// (`update_player_angle`). Then, calculates and updates the
// player's position (`update_player_position`). Adjusts movement speed
// based on delta_time to ensure consistent behavior regardless of frame rate.

void	move_player(t_ctrl *ctrl, double delta_time)
{
	float	move_speed;

	update_player_angle(&ctrl->game->player, delta_time);
	move_speed = 100.0f * delta_time;
	update_player_position(ctrl, move_speed);
}
