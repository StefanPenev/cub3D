/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:23 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 14:57:51 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	find_biggest_line(t_ctrl *ctrl)
{
	size_t	i;
	size_t	len;

	len = 0;
	i = 0;
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		if (ctrl->map.columns < len)
		{
			ctrl->map.columns = len;
		}
		i++;
	}
	printf("DEBUG: number of columns: %zu\n", ctrl->map.columns);
}

char	*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp)
{
	char	*new_map_arr;

	new_map_arr = ft_strjoin(map_tmp, line_tmp);
	if (!new_map_arr)
	{
		free(map_tmp);
		free(line_tmp);
		clean_exit(NULL, ctrl);
	}
	free(map_tmp);
	free(line_tmp);
	return (new_map_arr);
}
