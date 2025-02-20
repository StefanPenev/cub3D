/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:55:23 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/20 13:10:43 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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
			ctrl->map.columns = len;
		i++;
	}
}

char	*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp)
{
	char	*new_map_arr;

	new_map_arr = ft_strjoin(map_tmp, line_tmp);
	if (!new_map_arr)
	{
		free(map_tmp);
		free(line_tmp);
		clean_exit("Error: Memory allocation failed during merge_map_arr\n",
			ctrl);
	}
	free(map_tmp);
	return (new_map_arr);
}

void	free_and_exit(char *msg, char *line, char **tmp_arr, t_ctrl *ctrl)
{
	if (tmp_arr)
		free_map(tmp_arr, 3);
	if (line)
		free(line);
	if (ctrl->gnl.save)
		free(ctrl->gnl.save);
	if (ctrl->gnl.line)
		free(ctrl->gnl.line);
	if (ctrl->map.map_tmp)
		free(ctrl->map.map_tmp);
	clean_exit(msg, ctrl);
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

void	assign_enemy(t_enemy *enemy, int y, int x, int id)
{
	enemy->x = x * TILE_SIZE + TILE_SIZE / 2;
	enemy->y = y * TILE_SIZE + TILE_SIZE / 2;
	enemy->state = ENEMY_IDLE;
	enemy->frame = 0;
	enemy->frame_time = 0.0f;
	enemy->enemy_hp = ENEMY_HP;
	enemy->is_dead = 0;
	enemy->id = id;
}
