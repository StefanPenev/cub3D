/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/20 16:59:50 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static bool	all_textures_defined(t_ctrl *ctrl)
{
	return (ctrl->game->north_texture.path && ctrl->game->south_texture.path
		&& ctrl->game->west_texture.path && ctrl->game->east_texture.path);
}

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

void	keep_textures_path(int file_id, t_ctrl *ctrl)
{
	char	*line_tmp;
	int		texture_count;

	texture_count = 0;
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (!line_tmp)
			break ;
		if ((ft_strncmp("\n", line_tmp, 1) == 0))
		{
			if (texture_count == 4 && all_textures_defined(ctrl))
			{
				free(line_tmp);
				break ;
			}
			else
			{
				free(line_tmp);
				clean_exit("Wrong input:\nExactly 4 textures and empty line after them requires\n",
					ctrl);
			}
		}
		if (ft_strncmp("NO ", line_tmp, 3) == 0)
			ctrl->game->north_texture.path = ft_strdup(line_tmp + 3);
		else if (ft_strncmp("SO ", line_tmp, 3) == 0)
			ctrl->game->south_texture.path = ft_strdup(line_tmp + 3);
		else if (ft_strncmp("WE ", line_tmp, 3) == 0)
			ctrl->game->west_texture.path = ft_strdup(line_tmp + 3);
		else if (ft_strncmp("EA ", line_tmp, 3) == 0)
			ctrl->game->east_texture.path = ft_strdup(line_tmp + 3);
		else
		{
			free(line_tmp);
			clean_exit("Wrong input:\nUnexpected input in textures definition\n",
				ctrl);
		}
		texture_count++;
		if (texture_count > 4)
		{
			free(line_tmp);
			clean_exit("Wrong input:\nToo many textures\n", ctrl);
		}
		free(line_tmp);
	}
	if (texture_count != 4 || !all_textures_defined(ctrl))
		clean_exit("Wrong input:\nNot enough textures provided\n", ctrl);
	printf("DEBUG: north: %s\n", ctrl->game->north_texture.path);
	printf("DEBUG: south: %s\n", ctrl->game->south_texture.path);
	printf("DEBUG: west: %s\n", ctrl->game->west_texture.path);
	printf("DEBUG: east: %s\n", ctrl->game->east_texture.path);
}

int	is_color_valid(int i)
{
	if (i >= 0 && i <= 255)
	{
		return (1);
	}
	return (0);
}

void	define_FC_colors(int file_id, t_ctrl *ctrl)
{
	char	*line_tmp;
	int		colors_count;
	char	*clean_line_tmp;
	char	**tmp_arr;
	int		count;
	int		r;
	int		g;
	int		b;

	r = 0;
	g = 0;
	b = 0;
	colors_count = 0;
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (!line_tmp)
			break ;
		if (is_color_definition(line_tmp))
		{
			clean_line_tmp = ft_strdup(line_tmp + 3);
			tmp_arr = ft_split(clean_line_tmp, ',');
			count = 0;
			while (tmp_arr[count] != NULL)
			{
				count++;
			}
			if (count != 3)
			{
				free_map(tmp_arr, 3);
				free(clean_line_tmp);
				free(line_tmp);
				clean_exit("Wrong input:\nColor definition should contain exactly 3 integers\n",
					ctrl);
			}
			r = ft_atoi(tmp_arr[0]);
			if (is_color_valid(r) == 0)
			{
				free_map(tmp_arr, 3);
				free(clean_line_tmp);
				free(line_tmp);
				clean_exit("Wrong input:\nFloor R color is invalid\n", ctrl);
			}
			g = ft_atoi(tmp_arr[1]);
			if (is_color_valid(g) == 0)
			{
				free_map(tmp_arr, 3);
				free(clean_line_tmp);
				free(line_tmp);
				clean_exit("Wrong input:\nFloor G color is invalid\n", ctrl);
			}
			b = ft_atoi(tmp_arr[2]);
			if (is_color_valid(b) == 0)
			{
				free_map(tmp_arr, 3);
				free(clean_line_tmp);
				free(line_tmp);
				clean_exit("Wrong input:\nFloor B color is invalid\n", ctrl);
			}
			if (ft_strncmp("F ", line_tmp, 2) == 0)
			{
				ctrl->game->floor_color = (r << 16) | (g << 8) | b;
			}
			else if (ft_strncmp("C ", line_tmp, 2) == 0)
			{
				ctrl->game->ceiling_color = (r << 16) | (g << 8) | b;
			}
			colors_count++;
			free_map(tmp_arr, 3);
			free(clean_line_tmp);
			free(line_tmp);
		}
		if (colors_count == 2)
		{
			break ;
		}
		else
		{
			free(line_tmp);
			continue ;
		}
		free(line_tmp);
	}
	if (line_tmp == NULL && colors_count != 2)
	{
		clean_exit("Wrong input:\nNot enough colors provided\n", ctrl);
	}
	printf("DEBUG: floor_color: %u\n", ctrl->game->floor_color);
	printf("DEBUG: celling_color: %u\n", ctrl->game->ceiling_color);
}

char	*read_map(char *filename, t_ctrl *ctrl)
{
	int		file_id;
	char	*map_tmp;
	char	*line_tmp;

	map_tmp = ft_strdup("");
	if (!map_tmp)
		clean_exit(NULL, ctrl);
	file_id = open(filename, O_RDONLY);
	if (file_id == -1)
	{
		free(map_tmp);
		clean_exit(NULL, ctrl);
	}
	keep_textures_path(file_id, ctrl);
	define_FC_colors(file_id, ctrl);
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (line_tmp == NULL)
			break ;
		if (is_texture_definition(line_tmp))
		{
			keep_textures_path(file_id, ctrl);
		}
		else if (is_color_definition(line_tmp))
		{
			define_FC_colors(file_id, ctrl);
		}
		else if ((ft_strncmp("\n", line_tmp, 1) == 0))
		{
			free(line_tmp);
			continue ;
		}
		else
		{
			map_tmp = merge_map_arr(ctrl, line_tmp, map_tmp);
			if (!map_tmp)
			{
				free(line_tmp);
				free(map_tmp);
				clean_exit(NULL, ctrl);
			}
			ctrl->map.rows++;
		}
		free(line_tmp);
	}
	close(file_id);
	printf("DEBUG: number of rows: %zu\n", ctrl->map.rows);
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

	i = 0;
	find_biggest_line(ctrl);
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		printf("DEBUG: lenth of line %zu = %zu\n", i, len);
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
	char	*map_str;

	map_str = read_map(filename, ctrl);
	ctrl->map.full_map = ft_split(map_str, '\n');
	if (!ctrl->map.full_map)
	{
		free(map_str);
		clean_exit(NULL, ctrl);
	}
	free(map_str);
	explore_map(ctrl);
	check_valid_characters(ctrl);
	check_map_closed(ctrl);
	check_map_valid(ctrl);
}
