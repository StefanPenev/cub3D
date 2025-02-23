/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:18 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/23 14:53:07 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
