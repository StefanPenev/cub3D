/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:30:11 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 14:56:25 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (line_tmp == NULL)
			break ;
		map_tmp = merge_map_arr(ctrl, line_tmp, map_tmp);
		ctrl->map.rows++;
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
