/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/19 15:55:01 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Creates a deep copy of the game's map for validation purposes.
// - Allocates memory for the copy of the map.
// - Copies each row from the original map to the new map.
// - Handles memory allocation errors and exits if allocation fails.

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

// Recursively checks if the player has accessible paths
// in the map using flood fill.
// - Marks visited cells to avoid infinite loops.
// - Stops recursion when encountering walls or out-of-bounds indices.
// - Sets the `has_free_way` flag if a free path is found.

void	flood_fill(char **map, size_t x, size_t y, t_ctrl *ctrl)
{
	if (x >= ctrl->map.rows || y >= ctrl->map.columns)
		return ;
	if (map[x][y] == WALL || map[x][y] == 'F')
		return ;
	if (map[x][y] == EMPTY)
	{
		ctrl->map.has_free_way = 1;
		return ;
	}
	map[x][y] = 'F';
	flood_fill(map, x + 1, y, ctrl);
	flood_fill(map, x - 1, y, ctrl);
	flood_fill(map, x, y + 1, ctrl);
	flood_fill(map, x, y - 1, ctrl);
}

// Validates the entire map to ensure the player has accessible space to move.
// - Uses `create_map_copy` to generate a copy of the map for safe validation.
// - Calls `flood_fill` to explore accessible areas from the player's position.
// - Frees the copied map after validation.
// - Terminates the program if the player is trapped with no valid moves.

void	check_map_valid(t_ctrl *ctrl)
{
	char	**map_copy;
	size_t	x;
	size_t	y;

	x = ctrl->map.player_index.x;
	y = ctrl->map.player_index.y;
	map_copy = create_map_copy(ctrl);
	flood_fill(map_copy, x, y, ctrl);
	free_map(map_copy, ctrl->map.rows);
	if (ctrl->map.has_free_way == 0)
		clean_exit("Wrong input:\nPlayer has no space to move.\n", ctrl);
}
