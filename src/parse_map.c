/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/22 17:20:15 by anilchen         ###   ########.fr       */
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

int	is_texture_definition(char *line_tmp)
{
	if (ft_strncmp("NO ", line_tmp, 3) == 0 || ft_strncmp("SO ", line_tmp,
			3) == 0 || ft_strncmp("WE ", line_tmp, 3) == 0 || ft_strncmp("EA ",
			line_tmp, 3) == 0)
	{
		return (1);
	}
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

void	assign_path(char **str, char *path, t_ctrl *ctrl)
{
	if (*str != NULL)
	{
		free(path);
		clean_exit("Invalid input:\nTexture is duplicated\n", ctrl);
	}
	*str = path;
}

char	*remove_inner_spaces(char *str, t_ctrl *ctrl)
{
	char	*trimmed_str;

	trimmed_str = ft_strtrim(str, " ");
	if (!trimmed_str)
	{
		free(str);
		clean_exit("Memory allocation failed during trimming\n", ctrl);
	}
	free(str);
	return (trimmed_str);
}

void	free_and_exit(char *msg, char *line, char **tmp_arr, t_ctrl *ctrl)
{
	if (tmp_arr)
	{
		free_map(tmp_arr, 3);
	}
	if (line)
	{
		free(line);
	}
	clean_exit(msg, ctrl);
}

void	keep_textures_path(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	*path;
	char	*clean_path;
	int		len;

	trimmed_line = trim_trailing_whitespace(line);
	path = ft_strdup(trimmed_line + 3);
	if (!path)
		clean_exit("Memory allocation failed\n", ctrl);
	clean_path = remove_inner_spaces(path, ctrl);
	len = ft_strlen(clean_path);
	if (len < 4 || ft_strncmp(clean_path + len - 4, ".xpm", 4) != 0)
		free_and_exit("Invalid input:\nTexture should have .xpm extension.\n ",
			clean_path, NULL, ctrl);
	if (ft_strncmp("NO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->north_texture.path, clean_path, ctrl);
	else if (ft_strncmp("SO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->south_texture.path, clean_path, ctrl);
	else if (ft_strncmp("WE ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->west_texture.path, clean_path, ctrl);
	else if (ft_strncmp("EA ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->east_texture.path, clean_path, ctrl);
}

void	validate_rgb_line(char *clean_line_tmp, char *line, t_ctrl *ctrl)
{
	int	i;

	i = 0;
	while (clean_line_tmp[i] != '\0')
	{
		if (!ft_isdigit(clean_line_tmp[i]) && clean_line_tmp[i] != ','
			&& clean_line_tmp[i] != ' ')
		{
			free(clean_line_tmp);
			free_and_exit("Wrong input:\nInvalid color definition\n", line,
				NULL, ctrl);
		}
		i++;
	}
}

char	**split_and_check(char *line, t_ctrl *ctrl)
{
	int		i;
	char	*clean_line_tmp;
	char	**tmp_arr;
	char	*new_line;

	i = 0;
	clean_line_tmp = ft_strdup(line + 2);
	if (!clean_line_tmp)
		free_and_exit("Memory allocation failed\n", line, NULL, ctrl);
	validate_rgb_line(clean_line_tmp, line, ctrl);
	tmp_arr = ft_split(clean_line_tmp, ',');
	free(clean_line_tmp);
	if (!tmp_arr)
		free_and_exit("Memory allocation failed\n", line, NULL, ctrl);
	while (tmp_arr[i] != NULL)
	{
		new_line = remove_inner_spaces(tmp_arr[i], ctrl);
		tmp_arr[i] = new_line;
		i++;
	}
	if (i != 3)
		free_and_exit("Wrong input:\nInvalid color definition\n", line, tmp_arr,
			ctrl);
	return (tmp_arr);
}

void	define_fc_colors(char *line, t_ctrl *ctrl)
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
	tmp_arr = split_and_check(trimmed_line, ctrl);
	r = ft_atoi(tmp_arr[0]);
	g = ft_atoi(tmp_arr[1]);
	b = ft_atoi(tmp_arr[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		free_and_exit("Wrong input:\nRGB values must be between 0 and 255\n",
			trimmed_line, tmp_arr, ctrl);
	if (ft_strncmp("F ", trimmed_line, 2) == 0)
		ctrl->game->floor_color = (r << 16) | (g << 8) | b;
	else if (ft_strncmp("C ", trimmed_line, 2) == 0)
		ctrl->game->ceiling_color = (r << 16) | (g << 8) | b;
	else
		free_and_exit("Wrong input:\nUnknown color definition\n", trimmed_line,
			tmp_arr, ctrl);
	free_map(tmp_arr, 3);
}

static void	handle_line_based_on_state(char *trimmed_line, t_ctrl *ctrl,
		int *is_map_started)
{
	if (!(*is_map_started) && is_texture_definition(trimmed_line))
	{
		keep_textures_path(trimmed_line, ctrl);
		ctrl->map.textures_defined++;
	}
	else if (!(*is_map_started) && is_color_definition(trimmed_line))
	{
		define_fc_colors(trimmed_line, ctrl);
		ctrl->map.colors_defined++;
	}
	else if (ctrl->map.textures_defined >= 4 && ctrl->map.colors_defined >= 2)
	{
		*is_map_started = 1;
	}
	else if (!(*is_map_started))
	{
		if (ctrl->map.textures_defined < 4)
			clean_exit("Invalid input: Missing one or more textures.\n", ctrl);
		if (ctrl->map.colors_defined < 2)
			clean_exit("Invalid input: Missing floor or ceiling color.\n",
				ctrl);
	}
	else
		clean_exit("Invalid input: Unexpected line format.\n", ctrl);
}

static void	check_line_format(char *line, t_ctrl *ctrl, int *is_map_started)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t");
	if (!trimmed_line)
		clean_exit("Memory allocation failed\n", ctrl);
	if (ft_strlen(trimmed_line) == 0 || ft_strncmp("\n", line, 1) == 0)
	{
		free(trimmed_line);
		if (*is_map_started)
			clean_exit("Invalid input: Unexpected empty line inside map\n",
				ctrl);
		return ;
	}
	handle_line_based_on_state(trimmed_line, ctrl, is_map_started);
	free(trimmed_line);
}

// static void	handle_empty(char *trimmed_line, int *is_map_started,
//		t_ctrl *ctrl)
// {
// 	free(trimmed_line);
// 	if (*is_map_started)
// 		clean_exit("Invalid input: Unexpected empty line inside map\n", ctrl);
// }

// static void	handle_colors(char *trimmed_line, t_ctrl *ctrl)
// {
// 	define_fc_colors(trimmed_line, ctrl);
// 	ctrl->map.colors_defined++;
// 	free(trimmed_line);
// }

// static void	handle_textures(char *trimmed_line, t_ctrl *ctrl)
// {
// 	keep_textures_path(trimmed_line, ctrl);
// 	ctrl->map.textures_defined++;
// 	free(trimmed_line);
// }

// static void	check_line_format(char *line, t_ctrl *ctrl, int *is_map_started)
// {
// 	char	*trimmed_line;

// 	trimmed_line = ft_strtrim(line, " \t");
// 	if (!trimmed_line)
// 		clean_exit("Memory allocation failed\n", ctrl);
// 	if (ft_strlen(trimmed_line) == 0 || ft_strncmp("\n", line, 1) == 0)
// 	{
// 		handle_empty(trimmed_line, is_map_started, ctrl);
// 		return ;
// 	}
// 	if (!(*is_map_started) && is_texture_definition(trimmed_line))
// 	{
// 		handle_textures(trimmed_line, ctrl);
// 		return ;
// 	}
// 	if (!(*is_map_started) && is_color_definition(trimmed_line))
// 	{
// 		handle_colors(trimmed_line, ctrl);
// 		return ;
// 	}
// 	if (ctrl->map.textures_defined >= 4 && ctrl->map.colors_defined >= 2)
// 	{
// 		*is_map_started = 1;
// 		free(trimmed_line);
// 		return ;
// 	}
// 	if (!(*is_map_started))
// 	{
// 		if (ctrl->map.textures_defined < 4)
// 			clean_exit("Invalid input: Missing one or more textures.\n", ctrl);
// 		if (ctrl->map.colors_defined < 2)
// 			clean_exit("Invalid input: Missing floor or ceiling color.\n",
// 				ctrl);
// 	}
// 	free(trimmed_line);
// 	clean_exit("Invalid input: Unexpected line format.\n", ctrl);
// }

static char	*initialize_map_memory(t_ctrl *ctrl)
{
	char	*map_tmp;

	map_tmp = ft_strdup("");
	if (!map_tmp)
		clean_exit("Memory allocation failed\n", ctrl);
	return (map_tmp);
}

static void	process_line(char *line_tmp, char **map_tmp, int *is_map_started,
		t_ctrl *ctrl)
{
	check_line_format(line_tmp, ctrl, is_map_started);
	if (*is_map_started)
	{
		*map_tmp = merge_map_arr(ctrl, line_tmp, *map_tmp);
		ctrl->map.rows++;
	}
}

char	*read_map(char *filename, t_ctrl *ctrl)
{
	int		file_id;
	char	*line_tmp;
	char	*map_tmp;
	int		is_map_started;

	is_map_started = 0;
	map_tmp = initialize_map_memory(ctrl);
	file_id = open(filename, O_RDONLY);
	if (file_id == -1)
		clean_exit("Failed to open map file\n", ctrl);
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (!line_tmp)
			break ;
		process_line(line_tmp, &map_tmp, &is_map_started, ctrl);
		free(line_tmp);
		line_tmp = NULL;
	}
	close(file_id);
	if (!ctrl->map.rows)
		free_and_exit("Wrong input: Map is missing or empty\n", map_tmp, NULL,
			ctrl);
	// printf("DEBUG: number of rows: %zu\n", ctrl->map.rows);
	// printf("DEBUG: north: %s\n", ctrl->game->north_texture.path);
	// printf("DEBUG: south: %s\n", ctrl->game->south_texture.path);
	// printf("DEBUG: west: %s\n", ctrl->game->west_texture.path);
	// printf("DEBUG: east: %s\n", ctrl->game->east_texture.path);
	// printf("DEBUG: floor_color: %u\n", ctrl->game->floor_color);
	// printf("DEBUG: celling_color: %u\n", ctrl->game->ceiling_color);
	// printf("DEBUG: %s\n", map_tmp);
	return (map_tmp);
}

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

	// printf("DEBUG: Starting to parse map: %s\n", filename);
	map_tmp = read_map(filename, ctrl);
	// printf("DEBUG: Map string was successfully read and allocated\n");
	ctrl->map.full_map = ft_split(map_tmp, '\n');
	if (!ctrl->map.full_map)
	{
		// printf("DEBUG: Map splitting failed, freeing map_tmp\n");
		free(map_tmp);
		// printf("DEBUG: map_tmp WAS FREED\n");
		clean_exit("Error: Failed to split map string\n", ctrl);
	}
	// printf("DEBUG: Map string was split into full_map\n");
	free(map_tmp);
	// printf("DEBUG: map_tmp WAS FREED\n");
	// printf("DEBUG: Exploring map structure\n");
	explore_map(ctrl);
	// printf("DEBUG: Map exploration completed\n");
	// printf("DEBUG: Validating map characters\n");
	check_valid_characters(ctrl);
	// printf("DEBUG: Character validation completed\n");
	// printf("DEBUG: Checking if map is closed\n");
	check_map_closed(ctrl);
	// printf("DEBUG: Map closure check completed\n");
	// printf("DEBUG: Validating overall map correctness\n");
	check_map_valid(ctrl);
	// printf("DEBUG: Map validation completed\n");
	// printf("DEBUG: Map parsing completed successfully\n");
}
