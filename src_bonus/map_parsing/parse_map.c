/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/21 18:09:56 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Ensures each row of the map has the correct width by
// filling missing spaces with walls.
// - Allocates a new line with the correct width (ctrl->map.columns).
// - Copies the original row's content and fills the remaining part
// with WALL characters.
// - Frees the old line and replaces it with the new one.

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

// Adjusts all rows in the map to match the longest row's
// length and replaces empty spaces with walls.
// - Calls complete_map() to ensure consistent row length.
// - Replaces any ' ' (space) character in the map with a WALL
// character for consistency.

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

// Scans the map for door positions and creates an array of door objects.
// - Allocates memory for the doors array based on the number of doors found.
// - Populates the array with t_door structures, each representing a
// door's location and state.

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
				ctrl->map.doors[door_id++] = (t_door){j, i, 0, DOOR_CLOSED};
			j++;
		}
		i++;
	}
}

// Scans the map for enemy positions and creates an array of enemy objects.
//- Allocates memory for the enemies array based
// on the number of enemies found.
// - Initializes each enemy using assign_enemy() with its position and index.

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
				assign_enemy(&ctrl->map.enemies[enemy_index], i, j,
					enemy_index);
				enemy_index++;
			}
			j++;
		}
		i++;
	}
}

// Parses the map file and prepares all necessary map-related data structures.
// - Reads the map file into a temporary string.
// - Splits the map into rows and processes it with explore_map().
// - Validates the map structure and checks for valid characters and enclosure.
// - Initializes doors and enemies arrays if any are found during parsing.

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
		create_doors_array(ctrl);
	if (ctrl->map.enemies_counter > 0)
		create_enemy_array(ctrl);
}
