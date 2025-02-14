/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:38:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 15:50:45 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	handle_open_door(t_door *door, t_ctrl *ctrl, double delta_time)
{
	int	x;
	int	y;

	x = (int)(ctrl->game->player.x / TILE_SIZE);
	y = (int)(ctrl->game->player.y / TILE_SIZE);
	if (x == door->x && y == door->y)
	{
		door->timer = 3.0;
		return ;
	}
	else
	{
		door->timer -= delta_time;
		if (door->timer <= 0)
			door->state = DOOR_CLOSING;
	}
}

void	update_doors(t_door *door, t_ctrl *ctrl, double delta_time)
{
	if (door->state == DOOR_OPENING)
	{
		door->state = DOOR_OPEN;
		ctrl->map.full_map[door->y][door->x] = EMPTY;
		return ;
	}
	if (door->state == DOOR_OPEN)
		handle_open_door(door, ctrl, delta_time);
	if (door->state == DOOR_CLOSING)
	{
		door->state = DOOR_CLOSED;
		ctrl->map.full_map[door->y][door->x] = DOOR;
	}
}

// Retrieves the door object from the map based on grid coordinates
t_door	*get_door(int grid_x, int grid_y, t_map *map)
{
	size_t	i;

	i = 0;
	while (i < map->doors_counter)
	{
		if (map->doors[i].x == grid_x && map->doors[i].y == grid_y)
		{
			return (&map->doors[i]);
		}
		i++;
	}
	return (NULL);
}

void	door_state(t_ctrl *ctrl)
{
	t_door	*door;
	int		x;
	int		y;

	door = NULL;
	x = ctrl->game->player.x / TILE_SIZE;
	y = ctrl->game->player.y / TILE_SIZE;
	if (!in_map_bounds(x, y, &ctrl->map))
		return ;
	if (ctrl->map.full_map[y + 1][x] == DOOR)
		door = get_door(x, y + 1, &ctrl->map);
	else if (ctrl->map.full_map[y - 1][x] == DOOR)
		door = get_door(x, y - 1, &ctrl->map);
	else if (ctrl->map.full_map[y][x + 1] == DOOR)
		door = get_door(x + 1, y, &ctrl->map);
	else if (ctrl->map.full_map[y][x - 1] == DOOR)
		door = get_door(x - 1, y, &ctrl->map);
	if (!door)
		return ;
	if (door->state == DOOR_CLOSED)
	{
		door->state = DOOR_OPENING;
		door->timer = 3.0;
		return ;
	}
}
