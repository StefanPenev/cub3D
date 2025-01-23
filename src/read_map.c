/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:17:15 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/23 15:19:12 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
			free_and_exit("Wrong input:\nUnexpected empty line inside map\n",
				NULL, NULL, ctrl);
		return ;
	}
	handle_line_based_on_state(trimmed_line, ctrl, is_map_started);
	free(trimmed_line);
}

static char	*initialize_map_memory(t_ctrl *ctrl)
{
	char	*map_tmp;

	map_tmp = ft_strdup("");
	if (!map_tmp)
		clean_exit("Memory allocation failed\n", ctrl);
	return (map_tmp);
}

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
