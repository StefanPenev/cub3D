/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/23 15:17:40 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <ctype.h>

// static bool	data_completed(t_ctrl *ctrl)
// {
// 	return (ctrl->game->north_texture.path && ctrl->game->south_texture.path
// 		&& ctrl->game->west_texture.path && ctrl->game->east_texture.path
// 		&& ctrl->game->ceiling_color != 0xFFFFFFFF
// 		&& ctrl->game->floor_color != 0xFFFFFFFF);
// }

void	complete_map(t_ctrl *ctrl, size_t i, size_t len)
{
	char	*new_line;

	new_line = malloc(ctrl->map.columns + 1);
	if (!new_line)
	{
		clean_exit("Error allocating memory for map line", ctrl);
	}
	ft_memcpy(new_line, ctrl->map.full_map[i], len);
	ft_memset(new_line + len, WALL, ctrl->map.columns - len);
	new_line[ctrl->map.columns] = '\0';
	free(ctrl->map.full_map[i]);
	ctrl->map.full_map[i] = new_line;
}

void	explore_map(t_ctrl *ctrl)
{
	size_t	i;
	size_t	len;

	i = 0;
	find_biggest_line(ctrl);
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		if (len < ctrl->map.columns)
			complete_map(ctrl, i, len);
		len = 0;
		while (len < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][len] == ' ')
				ctrl->map.full_map[i][len] = WALL;
			len++;
		}
		i++;
	}
}

void	parse_map(char *filename, t_ctrl *ctrl)
{
	char	*map_tmp;

	map_tmp = read_map(filename, ctrl);
	ctrl->map.full_map = ft_split(map_tmp, '\n');
	if (!ctrl->map.full_map)
	{
		free(map_tmp);
		clean_exit("Error: Failed to split map string\n", ctrl);
	}
	free(map_tmp);
	explore_map(ctrl);
	check_valid_characters(ctrl);
	check_map_closed(ctrl);
	check_map_valid(ctrl);
}
