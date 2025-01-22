/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/22 14:22:06 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	**create_map_copy(t_ctrl *ctrl)
{
	char	**copy;
	size_t	i;

	i = 0;
	copy = malloc(ctrl->map.rows * sizeof(char *));
	while (i < ctrl->map.rows)
	{
		copy[i] = malloc(ctrl->map.columns + 1);
		if (!copy[i])
		{
			perror("Error allocating memory for map row copy");
			game_cleanup(ctrl);
			exit(1);
		}
		ft_strlcpy(copy[i], ctrl->map.full_map[i], ctrl->map.columns + 1);
		i++;
	}
	return (copy);
}

void	flood_fill(char **map, size_t x, size_t y, t_ctrl *ctrl)
{
	// Check if out of map boundaries
	if (x >= ctrl->map.rows || y >= ctrl->map.columns)
		return ;
	// If it's a wall or already visited cell, do nothing
	if (map[x][y] == WALL || map[x][y] == 'F')
		return ;
	// Check for free paths
	if (map[x][y] == EMPTY)
		ctrl->map.has_free_way = 1;
	// Mark the current cell as visited
	map[x][y] = 'F';
	// Recursively check adjacent cells
	flood_fill(map, x + 1, y, ctrl); // Down
	flood_fill(map, x - 1, y, ctrl); // Up
	flood_fill(map, x, y + 1, ctrl); // Right
	flood_fill(map, x, y - 1, ctrl); // Left
}

void	check_map_valid(t_ctrl *ctrl)
{
	char	**map_copy;
	size_t	x;
	size_t	y;

	//printf("DEBUG: Starting map validity check\n");
	x = ctrl->map.player_index.x;
	y = ctrl->map.player_index.y;
	//printf("DEBUG: Player initial position is (%zu, %zu)\n", x, y);
	map_copy = create_map_copy(ctrl);
	//printf("DEBUG: Map copy created successfully\n");
	flood_fill(map_copy, x, y, ctrl);
	//printf("DEBUG: Flood fill completed. Checking free way status\n");
	free_map(map_copy, ctrl->map.rows);
	//printf("DEBUG: Map copy memory freed\n");
	if (ctrl->map.has_free_way == 0)
	{
		//printf("ERROR:\nPlayer has no space to move\n");
		clean_exit("Invalid input:\nPlayer has no space to move.\n", ctrl);
	}
	//printf("DEBUG: Map validity check passed\n");
}
