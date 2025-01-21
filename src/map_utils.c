/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:23 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/21 16:16:43 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	find_biggest_line(t_ctrl *ctrl)
{
	size_t	i;
	size_t	len;

	len = 0;
	i = 0;
	printf("DEBUG: Starting to find the biggest line in the map\n");
	while (i < ctrl->map.rows)
	{
		if (!ctrl->map.full_map[i])
		{
			printf("ERROR: NULL line found at index %zu in map array\n", i);
			clean_exit("Error: NULL line found in map array", ctrl);
		}
		len = ft_strlen(ctrl->map.full_map[i]);
		printf("DEBUG: Line %zu length: %zu\n", i, len);
		if (ctrl->map.columns < len)
		{
			ctrl->map.columns = len;
			printf("DEBUG: Updated maximum columns to %zu\n",
				ctrl->map.columns);
		}
		i++;
	}
	printf("DEBUG: Finished finding the biggest line\n");
	printf("DEBUG: Final number of columns: %zu\n", ctrl->map.columns);
}

char	*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp)
{
	char	*new_map_arr;

	new_map_arr = ft_strjoin(map_tmp, line_tmp);
	// printf("DEBUG: new_map_arr WAS ALLOCATED TO REPLACE map_tmp\n");
	if (!new_map_arr)
	{
		free(map_tmp);
		printf("DEBUG: map_tmp WAS FREED\n");
		free(line_tmp);
		printf("DEBUG: line_tmp WAS FREED\n");
		clean_exit("Memory allocation failed during merge_map_arr\n", ctrl);
	}
	free(map_tmp);
	// printf("DEBUG: map_tmp WAS FREED\n");
	return (new_map_arr);
}
