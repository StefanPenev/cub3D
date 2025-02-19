/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:17:15 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/19 16:05:24 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Determines the type of line (texture, color,
//	or map) and processes it accordingly.
// - Handles texture path definitions if textures are not yet defined.
// - Handles floor/ceiling color definitions if colors are not yet defined.
//- Updates the map parsing state once all required textures
// and colors are defined.
//- Exits with an error if the line format is unexpected or
// required elements are missing.

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
		*is_map_started = 1;
	else if (!(*is_map_started))
	{
		if (ctrl->map.textures_defined < 4)
			free_and_exit("Wrong input:\nMissing one or more textures.\n",
				trimmed_line, NULL, ctrl);
		if (ctrl->map.colors_defined < 2)
			free_and_exit("Wrong input:\nMissing floor or ceiling color.\n",
				trimmed_line, NULL, ctrl);
	}
	else
		free_and_exit("Wrong input:\nUnexpected line format.\n", trimmed_line,
			NULL, ctrl);
}

// Validates the format of a single line from the map file.
// - Trims leading and trailing spaces from the line.
//- Checks for empty lines and handles them based
// on the current parsing state.
//- Passes the trimmed line to handle_line_based_on_state()
// for further processing.
// - Exits if memory allocation fails or line format is incorrect.

static void	check_line_format(char *line, t_ctrl *ctrl, int *is_map_started)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " ");
	if (!trimmed_line)
		clean_exit("Memory allocation failed\n", ctrl);
	if (ft_strlen(trimmed_line) == 0 || ft_strncmp("\n", line, 1) == 0)
	{
		free(trimmed_line);
		if (*is_map_started)
			free_and_exit("Wrong input:\nUnexpected empty line inside map\n",
				NULL, NULL, ctrl);
		return ;
	}
	handle_line_based_on_state(trimmed_line, ctrl, is_map_started);
	free(trimmed_line);
}

// Allocates and initializes an empty string for the map's temporary storage.
// - Returns an empty string to be used for accumulating map data.
// - Exits with an error if memory allocation fails.

static char	*initialize_map_memory(t_ctrl *ctrl)
{
	char	*map_tmp;

	map_tmp = ft_strdup("");
	if (!map_tmp)
		clean_exit("Memory allocation failed\n", ctrl);
	return (map_tmp);
}

// Processes a line based on the current parsing state of the map.
// - Checks the line's format using check_line_format().
// - If the map parsing has started,
//	appends the line to the map's temporary storage.
// - Increments the row count for each valid map line.

static void	process_line(char *line_tmp, int *is_map_started, t_ctrl *ctrl)
{
	check_line_format(line_tmp, ctrl, is_map_started);
	if (*is_map_started)
	{
		if (ctrl->map.map_tmp == NULL)
			ctrl->map.map_tmp = initialize_map_memory(ctrl);
		ctrl->map.map_tmp = merge_map_arr(ctrl, line_tmp, ctrl->map.map_tmp);
		ctrl->map.rows++;
	}
}

// Reads the entire map file line by line and processes its contents.
//- Opens the specified map file and reads it using a
// custom gnl (get_next_line) function.
// - For each line, calls process_line() to parse and store relevant data.
// - Handles file closing and memory cleanup after reading.
// - Exits with an error if the map file is empty or missing required content.

char	*read_map(char *filename, t_ctrl *ctrl)
{
	int		file_id;
	char	*line_tmp;
	int		is_map_started;

	is_map_started = 0;
	file_id = open(filename, O_RDONLY);
	if (file_id == -1)
		clean_exit("Failed to open map file\n", ctrl);
	while (1)
	{
		line_tmp = gnl(file_id, ctrl);
		if (!line_tmp)
			break ;
		process_line(line_tmp, &is_map_started, ctrl);
		free(line_tmp);
		ctrl->gnl.line = NULL;
		line_tmp = NULL;
	}
	close(file_id);
	if (!ctrl->map.rows)
		free_and_exit("Wrong input:\nMap is missing or empty\n",
			ctrl->map.map_tmp, NULL, ctrl);
	return (ctrl->map.map_tmp);
}
