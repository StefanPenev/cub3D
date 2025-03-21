/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 08:23:44 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 11:32:16 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "minimap.h"

static void	compute_minimap_data(t_map *map, t_game *game, t_minimap_data *data)
{
	data->block_size = TILE_SIZE / MINIMAP_SCALE;
	data->minimap_tiles = MINIMAP_SIZE / data->block_size;
	data->player_tile_x = (int)(game->player.x / TILE_SIZE);
	data->player_tile_y = (int)(game->player.y / TILE_SIZE);
	data->start_x = fmaxf(0, data->player_tile_x - data->minimap_tiles / 2.0f);
	data->start_y = fmaxf(0, data->player_tile_y - data->minimap_tiles / 2.0f);
	if (data->start_x + data->minimap_tiles > (int)map->columns)
		data->start_x = (int)map->columns - data->minimap_tiles;
	if (data->start_y + data->minimap_tiles > (int)map->rows)
		data->start_y = (int)map->rows - data->minimap_tiles;
}

static void	draw_player_marker(t_ctrl *ctrl, t_minimap_data *data)
{
	int		radius;
	int		index;
	float	cos_angle;
	float	sin_angle;

	data->x0 = MINIMAP_OFFSET_X + ((ctrl->game->player.x / TILE_SIZE)
			- data->start_x) * data->block_size;
	data->y0 = MINIMAP_OFFSET_Y + ((ctrl->game->player.y / TILE_SIZE)
			- data->start_y) * data->block_size;
	radius = data->block_size / 4;
	draw_circle(data->x0, data->y0, radius, ctrl->game);
	index = get_table_index(ctrl->game->player.angle);
	cos_angle = (float)ctrl->trig_tables->cos_table[index];
	sin_angle = (float)ctrl->trig_tables->sin_table[index];
	data->x1 = data->x0 + cos_angle * (data->block_size / 1.2);
	data->y1 = data->y0 + sin_angle * (data->block_size / 1.2);
	draw_minimap_line(data, ctrl->game);
}

static	void	draw_minimap_frame(t_game *game, t_minimap_data *data)
{
	int	minimap_width;
	int	minimap_height;
	int	end_x;
	int	end_y;

	minimap_width = data->minimap_tiles * data->block_size;
	minimap_height = minimap_width;
	end_x = MINIMAP_OFFSET_X + minimap_width;
	end_y = MINIMAP_OFFSET_Y + minimap_height;
	draw_minimap_edges(game, end_x, end_y);
}

void	draw_minimap(t_ctrl *ctrl)
{
	t_minimap_data	data;

	compute_minimap_data(&ctrl->map, ctrl->game, &data);
	draw_map_tiles(ctrl, &data);
	draw_player_marker(ctrl, &data);
	draw_minimap_frame(ctrl->game, &data);
}
