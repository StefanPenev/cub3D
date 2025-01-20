/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/20 13:55:31 by anilchen         ###   ########.fr       */
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
	// 	// Check if out of map boundaries
	if (x >= ctrl->map.rows || y >= ctrl->map.columns)
		return ;
	// 	// If it's a wall or already visited cell, do nothing
	if (map[x][y] == WALL || map[x][y] == 'F')
		return ;
	// 	// Check for free paths
	if (map[x][y] == EMPTY)
		ctrl->map.has_free_way = 1;
	// 	// Mark the current cell as visited
	map[x][y] = 'F';
	// 		// Recursively check adjacent cells
	flood_fill(map, x + 1, y, ctrl); // Down
	flood_fill(map, x - 1, y, ctrl); // Up
	flood_fill(map, x, y + 1, ctrl); // Right
	flood_fill(map, x, y - 1, ctrl); // Left
}

// void	check_map_valid(t_ctrl *ctrl)
// {
// 	char	**map_copy;
// 	size_t	x;
// 	size_t	y;
// 	size_t	count;

// 	count = 0;
// 	while (count < ctrl->map.players_count)
// 	{
// 		x = ctrl->map.players_positions[count].x;
// 		y = ctrl->map.players_positions[count].y;
// 		map_copy = create_map_copy(ctrl);
// 		flood_fill(map_copy, x, y, ctrl);
// 		free_map(map_copy, ctrl->map.rows);
// 		if (ctrl->map.has_free_way == 0)
// 			clean_exit("Invalid input:\nPlayer has no space to move.\n", ctrl);
// 		count++;
// 	}
// }

void	check_map_valid(t_ctrl *ctrl)
{
	char	**map_copy;
	size_t	x;
	size_t	y;

		x = ctrl->map.player_position.x;
		y = ctrl->map.player_position.y;
		map_copy = create_map_copy(ctrl);
		flood_fill(map_copy, x, y, ctrl);
		free_map(map_copy, ctrl->map.rows);
		if (ctrl->map.has_free_way == 0)
			clean_exit("Invalid input:\nPlayer has no space to move.\n", ctrl);

}

