/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:38:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/03 16:25:53 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"


void	update_doors(t_door *door, t_map *map, double delta_time)
{
	if (delta_time < 0.01)
		delta_time = 0.01;
	if (door->state == DOOR_OPENING)
	{
		if (door->offset < TILE_SIZE)
		{
			door->offset += 100 * delta_time;
			//printf("DEBUG: Updating door at (%d, %d) | offset = %.2f\n",
			//door->x, door->y, door->offset);
		}
		if (door->offset >= TILE_SIZE)
		{
			door->state = DOOR_OPEN;
			door->offset = TILE_SIZE;
			map->full_map[door->y][door->x] = EMPTY;
            printf("%s\n", map->full_map[door->y]);
			printf("DEBUG: Door fully open at (%d, %d)\n", door->x, door->y);
		}
	}
}

t_door	*get_door(int grid_x, int grid_y, t_map *map)
{
	size_t	i;

	i = 0;
	while (i < map->doors_counter)
	{
		if (map->doors[i].x == grid_x && map->doors[i].y == grid_y)
		{
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
			return (&map->doors[i]);
		}
		i++;
	}
	return (NULL);
}

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
		door->state = DOOR_OPENING;
	}
	else
	{
		printf("DEBUG: No door found at (%d, %d)\n", grid_x, grid_y);
	}
}
