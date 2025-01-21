/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/21 16:46:40 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <ctype.h>

static bool	data_completed(t_ctrl *ctrl)
{
	return (ctrl->game->north_texture.path && ctrl->game->south_texture.path
		&& ctrl->game->west_texture.path && ctrl->game->east_texture.path
		&& ctrl->game->ceiling_color != 0xFFFFFFFF
		&& ctrl->game->floor_color != 0xFFFFFFFF);
}

int	is_texture_definition(char *line_tmp)
{
	if (ft_strncmp("NO ", line_tmp, 3) == 0 || ft_strncmp("SO ", line_tmp,
			3) == 0 || ft_strncmp("WE ", line_tmp, 3) == 0 || ft_strncmp("EA ",
			line_tmp, 3) == 0)
		return (1);
	return (0);
}

int	is_color_definition(char *line_tmp)
{
	if (ft_strncmp("F ", line_tmp, 2) == 0 || ft_strncmp("C ", line_tmp,
			2) == 0)
	{
		return (1);
	}
	return (0);
}

void	keep_textures_path(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	*path;
	int		len;
	int		i;
	char	*trimmed_path;

	i = 0;
	trimmed_line = trim_trailing_whitespace(line);
	printf("DEBUG: Processing trimmed line: %s\n", trimmed_line);
	path = ft_strdup(trimmed_line + 3);
	if (!path)
		clean_exit("Memory allocation failed\n", ctrl);
	while (path[i] != '\0')
	{
		if (ft_isspace(path[i]))
		{
			trimmed_path = ft_strtrim(path, " ");
			if (!trimmed_path)
			{
				free(path);
				clean_exit("Memory allocation failed during trimming\n", ctrl);
			}
			free(path);
			path = trimmed_path;
		}
		i++;
	}
	printf("PATH WAS ALLOCATED\n");
	printf("DEBUG: Extracted path: %s\n", path);
	len = ft_strlen(path);
	printf("DEBUG: Path length: %d\n", len);
	if (len < 4 || ft_strncmp(path + len - 4, ".xpm", 4) != 0)
	{
		printf("ERROR: Invalid texture format. Path: %s\n", path);
		free(path);
		clean_exit("Wrong format of textures: must end with .xpm\n", ctrl);
	}
	if (ft_strncmp("NO ", trimmed_line, 3) == 0)
	{
		ctrl->game->north_texture.path = path;
		printf("PATH WAS ASSIGNED TO NORTH AND SHOULDNT BE FREED AT THE MOMENT\n");
	}
	else if (ft_strncmp("SO ", trimmed_line, 3) == 0)
	{
		ctrl->game->south_texture.path = path;
		printf("PATH WAS ASSIGNED TO SOUTH AND SHOULDNT BE FREED AT THE MOMENT\n");
	}
	else if (ft_strncmp("WE ", trimmed_line, 3) == 0)
	{
		ctrl->game->west_texture.path = path;
		printf("PATH WAS ASSIGNED TO WEST AND SHOULDNT BE FREED AT THE MOMENT\n");
	}
	else if (ft_strncmp("EA ", trimmed_line, 3) == 0)
	{
		ctrl->game->east_texture.path = path;
		printf("PATH WAS ASSIGNED TO EAST AND SHOULDNT BE FREED AT THE MOMENT\n");
	}
	else
	{
		printf("ERROR: Unexpected prefix in line: %s\n", trimmed_line);
		free(path);
		printf("PATH WAS FREED\n");
		clean_exit("Wrong texture definition: unexpected prefix\n", ctrl);
	}
}

void	free_and_exit(char *msg, char *line, char **tmp_arr, t_ctrl *ctrl)
{
	if (tmp_arr)
	{
		printf("DEBUG: Freeing tmp_arr\n");
		free_map(tmp_arr, 3);
		printf("DEBUG: tmp_arr WAS FREED\n");
	}
	if (line)
	{
		printf("DEBUG: Freeing line\n");
		free(line);
		printf("DEBUG: line WAS FREED\n");
	}
	printf("DEBUG: Exiting with message: %s\n", msg);
	clean_exit(msg, ctrl);
}

char	**split_and_check(char *line, t_ctrl *ctrl)
{
	char	*clean_line_tmp;
	int		i;
	char	**tmp_arr;
	int		j;
	char	*new_line;

	j = 0;
	// printf("DEBUG: Processing line: %s\n", line);
	i = 0;
	clean_line_tmp = ft_strdup(line + 2);
	printf("clean_line_tmp WAS ALLOCATED\n");
	if (!clean_line_tmp)
	{
		printf("ERROR: Memory allocation failed for clean_line_tmp\n");
		free_and_exit("Memory allocation fail\n", line, NULL, ctrl);
	}
	// printf("DEBUG: Extracted clean line: %s\n", clean_line_tmp);
	while (clean_line_tmp[i] != '\0')
	{
		if (!ft_isdigit(clean_line_tmp[i]) && clean_line_tmp[i] != ','
			&& clean_line_tmp[i] != ' ')
		{
			printf("ERROR: Invalid character '%c' in clean line\n",
				clean_line_tmp[i]);
			free(clean_line_tmp);
			printf("clean_line_tmp WAS FREED\n");
			free_and_exit("Wrong input:\nInvalid color definition\n", line,
				NULL, ctrl);
		}
		i++;
	}
	tmp_arr = ft_split(clean_line_tmp, ',');
	printf("tmp_arr WAS ALLOCATED\n");
	free(clean_line_tmp);
	printf("clean_line_tmp WAS FREED\n");
	if (!tmp_arr)
	{
		printf("ERROR: Memory allocation failed for tmp_arr\n");
		free_and_exit("Memory allocation fail\n", line, NULL, ctrl);
	}
	i = 0;
	while (tmp_arr[i] != NULL)
	{
		j = 0;
		while (tmp_arr[i][j] != '\0')
		{
			if (ft_isspace(tmp_arr[i][j]) || tmp_arr[i][j] == '\t')
			{
				new_line = ft_strtrim(tmp_arr[i], " \t");
				if (!new_line)
				{
					free(tmp_arr[i]);
					free_and_exit("Memory allocation failed during trimming\n",
						line, tmp_arr, ctrl);
				}
				free(tmp_arr[i]);
				tmp_arr[i] = new_line;
				break ; 
			}
			j++;
		}
		i++;
	}
	// printf("DEBUG: Number of elements in tmp_arr: %d\n", i);
	if (i != 3)
	{
		printf("ERROR: Expected 3 elements in tmp_arr, got %d\n", i);
		free_and_exit("Wrong input:\nInvalid color definition\n", line, tmp_arr,
			ctrl);
	}
	// printf("DEBUG: Successfully split line into RGB values\n");
	return (tmp_arr);
}

void	define_FC_colors(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	**tmp_arr;
	int		r;
	int		g;
	int		b;

	r = 0;
	g = 0;
	b = 0;
	trimmed_line = trim_trailing_whitespace(line);
	printf("DEBUG: Processing trimmed line: %s\n", trimmed_line);
	tmp_arr = split_and_check(trimmed_line, ctrl);
	r = ft_atoi(tmp_arr[0]);
	g = ft_atoi(tmp_arr[1]);
	b = ft_atoi(tmp_arr[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_and_exit("Wrong input:\nRGB values must be between 0 and 255\n",
			trimmed_line, tmp_arr, ctrl);
	}
	if (ft_strncmp("F ", trimmed_line, 2) == 0)
	{
		ctrl->game->floor_color = (r << 16) | (g << 8) | b;
		printf("DEBUG: Floor color set to: %d\n", ctrl->game->floor_color);
	}
	else if (ft_strncmp("C ", trimmed_line, 2) == 0)
	{
		ctrl->game->ceiling_color = (r << 16) | (g << 8) | b;
		printf("DEBUG: Ceiling color set to: %d\n", ctrl->game->ceiling_color);
	}
	else
	{
		free_and_exit("Wrong input:\nUnknown color definition\n", trimmed_line,
			tmp_arr, ctrl);
	}
	free_map(tmp_arr, 3);
	printf("tmp_arr WAS FREED\n");
}

static void	check_line_format(char *line, t_ctrl *ctrl, int *is_map_started)
{
	if (ft_strlen(line) == 0 || ft_strncmp("\n", line, 1) == 0)
		return ;
	if (!(*is_map_started) && is_texture_definition(line))
	{
		keep_textures_path(line, ctrl);
		ctrl->map.textures_defined++;
		return ;
	}
	if (!(*is_map_started) && is_color_definition(line))
	{
		define_FC_colors(line, ctrl);
		ctrl->map.colors_defined++;
		return ;
	}
	if (ctrl->map.textures_defined >= 4 && ctrl->map.colors_defined >= 2
		&& ft_strncmp("\n", line, 1) != 0)
	{
		*is_map_started = 1;
		return ;
	}
	clean_exit("Invalid input: Unexpected line format.\n", ctrl);
}

char	*read_map(char *filename, t_ctrl *ctrl)
{
	int		file_id;
	char	*line_tmp;
	char	*map_tmp;
	int		is_map_started;

	is_map_started = 0;
	map_tmp = ft_strdup("");
	printf("map_tmp WAS ALLOCATED\n");
	if (!map_tmp)
		clean_exit("Memory allocation failed\n", ctrl);
	file_id = open(filename, O_RDONLY);
	if (file_id == -1)
		clean_exit("Failed to open map file\n", ctrl);
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (!line_tmp)
			break ;
		check_line_format(line_tmp, ctrl, &is_map_started);
		if (is_map_started)
		{
			map_tmp = merge_map_arr(ctrl, line_tmp, map_tmp);
			ctrl->map.rows++;
		}
		free(line_tmp);
		line_tmp = NULL;
	}
	close(file_id);
	if (!ctrl->map.rows)
	{
		free(map_tmp);
		clean_exit("Wrong input: Map is missing or empty\n", ctrl);
	}
	if (!data_completed(ctrl))
	{
		free(map_tmp);
		clean_exit("Wrong input: Colors or textures are missing or empty\n",
			ctrl);
	}
	printf("DEBUG: number of rows: %zu\n", ctrl->map.rows);
	printf("DEBUG: north: %s\n", ctrl->game->north_texture.path);
	printf("DEBUG: south: %s\n", ctrl->game->south_texture.path);
	printf("DEBUG: west: %s\n", ctrl->game->west_texture.path);
	printf("DEBUG: east: %s\n", ctrl->game->east_texture.path);
	printf("DEBUG: floor_color: %u\n", ctrl->game->floor_color);
	printf("DEBUG: celling_color: %u\n", ctrl->game->ceiling_color);
	printf("DEBUG: %s\n", map_tmp);
	return (map_tmp);
}

void	complete_map(t_ctrl *ctrl, size_t i, size_t len)
{
	char	*new_line;

	new_line = malloc(ctrl->map.columns + 1);
	if (!new_line)
	{
		perror("Error allocating memory for map line");
		exit(1);
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

	printf("DEBUG: Starting map exploration\n");
	i = 0;
	find_biggest_line(ctrl);
	printf("DEBUG: Map biggest line length: %zu\n", ctrl->map.columns);
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		printf("DEBUG: Length of line %zu = %zu\n", i, len);
		if (len < ctrl->map.columns)
		{
			printf("DEBUG: Completing line %zu to match the longest line\n", i);
			complete_map(ctrl, i, len);
			printf("DEBUG: Line %zu completed\n", i);
		}
		len = 0;
		while (len < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][len] == ' ')
			{
				ctrl->map.full_map[i][len] = WALL;
				printf("DEBUG: Replaced space with WALL at [%zu][%zu]\n", i,
					len);
			}
			len++;
		}
		i++;
	}
	printf("DEBUG: Map exploration completed successfully\n");
}

void	parse_map(char *filename, t_ctrl *ctrl)
{
	char	*map_tmp;

	printf("DEBUG: Starting to parse map: %s\n", filename);
	map_tmp = read_map(filename, ctrl);
	printf("DEBUG: Map string was successfully read and allocated\n");
	ctrl->map.full_map = ft_split(map_tmp, '\n');
	if (!ctrl->map.full_map)
	{
		printf("DEBUG: Map splitting failed, freeing map_tmp\n");
		free(map_tmp);
		printf("DEBUG: map_tmp WAS FREED\n");
		clean_exit("Error: Failed to split map string\n", ctrl);
	}
	printf("DEBUG: Map string was split into full_map\n");
	free(map_tmp);
	printf("DEBUG: map_tmp WAS FREED\n");
	printf("DEBUG: Exploring map structure\n");
	explore_map(ctrl);
	printf("DEBUG: Map exploration completed\n");
	printf("DEBUG: Validating map characters\n");
	check_valid_characters(ctrl);
	printf("DEBUG: Character validation completed\n");
	printf("DEBUG: Checking if map is closed\n");
	check_map_closed(ctrl);
	printf("DEBUG: Map closure check completed\n");
	printf("DEBUG: Validating overall map correctness\n");
	check_map_valid(ctrl);
	printf("DEBUG: Map validation completed\n");
	printf("DEBUG: Map parsing completed successfully\n");
}
