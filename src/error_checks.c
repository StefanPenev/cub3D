/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:52:25 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 14:27:55 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

void	find_players_pos(t_ctrl *ctrl, size_t i, size_t j)
{
	char	c;
	int		count;

	count = 0;
	i = 0;
	j = 0;
	ctrl->map.players_positions = malloc(ctrl->map.players_count
			* sizeof(t_pos));
	while (i < ctrl->map.rows)
	{
		while (j < ctrl->map.columns)
		{
			c = ctrl->map.full_map[i][j];
			if (c == NORTH || c == SOUTH || c == EAST || c == WEST)
			{
				ctrl->map.players_positions[count].x = i;
				ctrl->map.players_positions[count].y = j;
				count++;
			}
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
					|| c == EAST || c == WEST))
				clean_exit("Invalid input:\nMap contains invalid characters.\n",
					ctrl);
			if (c == NORTH || c == SOUTH || c == EAST || c == WEST)
				ctrl->map.players_count++;
			j++;
		}
		i++;
	}
	if (ctrl->map.players_count == 0)
		clean_exit("Invalid input:\nPlayer position not found.\n", ctrl);
	else
		find_players_pos(ctrl, i, j);
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
			clean_exit("Invalid input:\nMap must be closed by wall.\n", ctrl);
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
