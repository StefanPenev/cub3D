/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:10:00 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 16:39:18 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Checks if a position is valid for movement and handles door opening
static int	is_valid_position(float x, float y, t_map *map)
{
	size_t	grid_x;
	size_t	grid_y;
	char	tile;
	t_door	*door;

	// Ensure the position is within map boundaries
	if (!in_map_bounds(x, y, map))
		return (0);
	// Convert floating-point position to grid coordinates
	grid_x = (size_t)(x / TILE_SIZE);
	grid_y = (size_t)(y / TILE_SIZE);
	tile = map->full_map[grid_y][grid_x];
	// Check for nearby doors and get door object if present
	if (map->full_map[grid_y + 1][grid_x] == DOOR)
		door = get_door(grid_x, grid_y + 1, map);
	else if (map->full_map[grid_y - 1][grid_x] == DOOR)
		door = get_door(grid_x, grid_y - 1, map);
	else if (map->full_map[grid_y][grid_x + 1] == DOOR)
		door = get_door(grid_x + 1, grid_y, map);
	else if (map->full_map[grid_y][grid_x - 1] == DOOR)
		door = get_door(grid_x - 1, grid_y, map);
	else
		door = NULL;
	// If a closed door is detected, open it
	if (door && door->state == DOOR_CLOSED)
	{
		printf("hello from if\n");
		printf("DEBUG: Opening door at (%d, %d)\n", door->x, door->y);
		door_open(door->x, door->y, map);
	}
	// Return whether the tile is walkable (valid for movement)
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
