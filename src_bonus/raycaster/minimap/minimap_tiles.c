/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_tiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:31:01 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 17:35:20 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes_bonus/cub3d.h"
#include "minimap.h"

static void	draw_tile_base(t_ctrl *ctrl, t_minimap_data *data, int x, int y)
{
	int	map_x;
	int	map_y;
	int	color;

	map_x = (int)(data->start_x + x);
	map_y = (int)(data->start_y + y);
	color = FLOOR_COLOR;
	if (ctrl->map.full_map[map_y][map_x] == '1')
		color = WALL_COLOR;
	draw_square((t_square){
		MINIMAP_OFFSET_X + x * data->block_size,
		MINIMAP_OFFSET_Y + y * data->block_size,
		data->block_size, color}, ctrl->game);
}

static void	draw_tile_enemy(t_ctrl *ctrl, t_minimap_data *data, int x, int y)
{
	int		map_x;
	int		map_y;
	int		color;
	t_enemy	*enemy;

	map_x = (int)(data->start_x + x);
	map_y = (int)(data->start_y + y);
	if (ctrl->map.full_map[map_y][map_x] == 'M'
		|| ctrl->map.full_map[map_y][map_x] == 'C')
	{
		enemy = get_enemy(map_x, map_y, &ctrl->map);
		if (enemy != NULL && enemy->is_dead)
		{
			ctrl->map.full_map[map_y][map_x] = 'C';
			color = CORPSE_COLOR;
		}
		else
			color = ENEMY_COLOR;
		draw_square((t_square){
			MINIMAP_OFFSET_X + x * data->block_size + data->block_size / 4,
			MINIMAP_OFFSET_Y + y * data->block_size + data->block_size / 4,
			data->block_size / 2, color}, ctrl->game);
	}
}

static void	process_map_tile(t_ctrl *ctrl, t_minimap_data *data, int x, int y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(data->start_x + x);
	map_y = (int)(data->start_y + y);
	if (map_x >= 0 && map_x < (int)ctrl->map.columns && map_y >= 0
		&& map_y < (int)ctrl->map.rows)
	{
		draw_tile_base(ctrl, data, x, y);
		draw_tile_enemy(ctrl, data, x, y);
	}
}

void	draw_map_tiles(t_ctrl *ctrl, t_minimap_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->minimap_tiles)
	{
		x = 0;
		while (x < data->minimap_tiles)
		{
			process_map_tile(ctrl, data, x, y);
			x++;
		}
		y++;
	}
}
