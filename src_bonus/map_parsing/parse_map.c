/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/07 11:29:39 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

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

void	create_box_array(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	int		box_id;

	box_id = 0;
	i = 0;
	ctrl->map.boxes = malloc(ctrl->map.box_counter * sizeof(t_box));
	if (!ctrl->map.boxes)
		clean_exit("Memory allocation failed for map->boxes\n", ctrl);
	while (i < ctrl->map.rows)
	{
		j = 0;
		while (j < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][j] == COLLECTIBLE)
			{
				ctrl->map.boxes[box_id].x = j;
				ctrl->map.boxes[box_id].y = i;
				ctrl->map.boxes[box_id].state = NOT_DAMAGED;
				printf("DEBUG: Box added at (%zu, %zu) at index %d\n", j, i,
					box_id);
				box_id++;
			}
			j++;
		}
		i++;
	}
}


void	create_doors_array(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	int		door_id;

	door_id = 0;
	i = 0;
	ctrl->map.doors = malloc(ctrl->map.doors_counter * sizeof(t_door));
	if (!ctrl->map.doors)
		clean_exit("Memory allocation failed for map->doors\n", ctrl);
	while (i < ctrl->map.rows)
	{
		j = 0;
		while (j < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][j] == DOOR)
			{
				ctrl->map.doors[door_id].x = j;
				ctrl->map.doors[door_id].y = i;
				//ctrl->map.doors[door_id].offset = 0;
				ctrl->map.doors[door_id].timer = 0;
				ctrl->map.doors[door_id].state = DOOR_CLOSED;
				printf("DEBUG: Door added at (%zu, %zu) at index %d\n", j, i,
					door_id);
				door_id++;
			}
			j++;
		}
		i++;
	}
}

void	create_enemy_array(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	size_t	enemy_index;

	enemy_index = 0;
	ctrl->map.enemies = malloc(sizeof(t_enemy) * ctrl->map.enemies_counter);
	if (!ctrl->map.enemies)
		clean_exit("Error: Memory allocation failed for enemies.\n", ctrl);
	i = 0;
	while (i < ctrl->map.rows)
	{
		j = 0;
		while (j < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][j] == ENEMY)
			{
				ctrl->map.enemies[enemy_index].x = j * TILE_SIZE
					+ TILE_SIZE / 2;
				ctrl->map.enemies[enemy_index].y = i * TILE_SIZE
					+ TILE_SIZE / 2;
				ctrl->map.enemies[enemy_index].state = ENEMY_IDLE;
				ctrl->map.enemies[enemy_index].frame = 0;
				ctrl->map.enemies[enemy_index].frame_time = 0.0f;
				enemy_index++;
			}
			j++;
		}
		i++;
	}
	printf("Enemies successfully allocated and positioned.\n");
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
	if (ctrl->map.doors_counter > 0)
	{
		create_doors_array(ctrl);
	}
	if (ctrl->map.box_counter > 0)
	{
		create_box_array(ctrl);
	}
	if (ctrl->map.enemies_counter > 0)
	{
		create_enemy_array(ctrl);
	}
}
