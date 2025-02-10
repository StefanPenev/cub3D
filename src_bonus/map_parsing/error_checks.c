/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:52:25 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/10 15:01:05 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	check_args(int argc, char *argv[], t_ctrl *ctrl)
{
	size_t	len;

	len = 0;
	if (argc < 2)
		clean_exit("Invalid input:\nThis game requires a map.\n", ctrl);
	if (argc > 2)
		clean_exit("Invalid input:\nThis game requires only one argument.\n",
			ctrl);
	len = ft_strlen(argv[1]);
	if (len < 4 || ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0)
		clean_exit("Invalid input:\nMapname doesn't have a .cub extension.\n",
			ctrl);
}

void	save_position(t_ctrl *ctrl, size_t i, size_t j, char c)
{
	ctrl->map.player_index.x = i;
	ctrl->map.player_index.y = j;
	ctrl->map.player_position.x = (j * TILE_SIZE) + (TILE_SIZE / 2);
	ctrl->map.player_position.y = (i * TILE_SIZE) + (TILE_SIZE / 2);
	if (c == 'N')
		ctrl->map.player_position.orientation = 3 * M_PI / 2;
	else if (c == 'S')
		ctrl->map.player_position.orientation = M_PI / 2;
	else if (c == 'E')
		ctrl->map.player_position.orientation = 0;
	else if (c == 'W')
		ctrl->map.player_position.orientation = M_PI;
}

void	find_players_pos(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	j = 0;
	while (i < ctrl->map.rows)
	{
		while (j < ctrl->map.columns)
		{
			c = ctrl->map.full_map[i][j];
			if (c == NORTH || c == SOUTH || c == EAST || c == WEST)
				save_position(ctrl, i, j, c);
			j++;
		}
		j = 0;
		i++;
	}
}

void	check_valid_characters(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (i < ctrl->map.rows)
	{
		j = 0;
		while (j < ctrl->map.columns)
		{
			c = ctrl->map.full_map[i][j];
			if (!(c == WALL || c == EMPTY || c == NORTH || c == SOUTH
					|| c == EAST || c == WEST || c == DOOR || c == ENEMY))
				clean_exit("Invalid map: contains unknown characters.\n", ctrl);
			ctrl->map.players_count += (c == NORTH || c == SOUTH || c == EAST
					|| c == WEST);
			ctrl->map.doors_counter += (c == DOOR);
			ctrl->map.enemies_counter += (c == ENEMY);
			j++;
		}
		i++;
	}
	if (ctrl->map.players_count != 1)
		clean_exit("Invalid input:\nSingle player position expected.\n", ctrl);
	find_players_pos(ctrl);
}

void	check_map_closed(t_ctrl *ctrl)
{
	size_t	i;

	i = 0;
	while (i < ctrl->map.columns)
	{
		if (ctrl->map.full_map[0][i] != WALL)
			clean_exit("Invalid input:\nMap must be closed by walls.\n", ctrl);
		if (ctrl->map.full_map[ctrl->map.rows - 1][i] != WALL)
			clean_exit("Invalid input:\nMap must be closed by walls.\n", ctrl);
		i++;
	}
	i = 0;
	while (i < ctrl->map.rows)
	{
		if (ctrl->map.full_map[i][0] != WALL)
			clean_exit("Invalid input:\nMap must be closed by walls.\n", ctrl);
		if (ctrl->map.full_map[i][ctrl->map.columns - 1] != WALL)
			clean_exit("Invalid input:\nMap must be closed by walls.\n", ctrl);
		i++;
	}
}
