/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:38:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/04 12:51:25 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

// Updates the door state and smoothly moves it over time
void	update_doors(t_door *door, t_map *map, double delta_time)
{
	if (delta_time < 0.01)
		delta_time = 0.01;  // Prevents extremely small delta_time values

	if (door->state == DOOR_OPENING)
	{
		if (door->offset < TILE_SIZE)
		{
			door->offset += 100 * delta_time;  // Moves the door smoothly
		}
		if (door->offset >= TILE_SIZE)
		{
			door->state = DOOR_OPEN;  // Marks the door as fully opened
			door->offset = TILE_SIZE;  // Ensures offset doesn't exceed TILE_SIZE
			map->full_map[door->y][door->x] = EMPTY;  // Replaces door tile with empty space
			printf("DEBUG: Door fully open at (%d, %d)\n", door->x, door->y);
		}
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
			// Determines door orientation (horizontal or vertical)
			if (grid_x > 0 && grid_x < (int)map->columns - 1)
			{
				if (map->full_map[grid_y][grid_x - 1] == WALL
					&& map->full_map[grid_y][grid_x + 1] == WALL)
				{
					map->doors[i].orientation = HORIZONTAL;
				}
			}
			if (grid_y > 0 && grid_y < (int)map->rows - 1)
			{
				if (map->full_map[grid_y - 1][grid_x] == WALL
					&& map->full_map[grid_y + 1][grid_x] == WALL)
				{
					map->doors[i].orientation = VERTICAL;
				}
			}
			return (&map->doors[i]);  // Returns the door if found
		}
		i++;
	}
	return (NULL);  // Returns NULL if no door is found
}

// Opens the door at the specified grid coordinates
void	door_open(int grid_x, int grid_y, t_map *map)
{
	t_door	*door;

	printf("DEBUG: Checking door at (%d, %d) | map->full_map = %c\n", grid_x,
		grid_y, map->full_map[grid_y][grid_x]);

	door = get_door(grid_x, grid_y, map);
	if (door)
	{
		printf("DEBUG: Door found at (%d, %d), changing state to OPENING\n",
			grid_x, grid_y);
		door->state = DOOR_OPENING;  // Starts the door opening animation
	}
	else
	{
		printf("DEBUG: No door found at (%d, %d)\n", grid_x, grid_y);
	}
}
