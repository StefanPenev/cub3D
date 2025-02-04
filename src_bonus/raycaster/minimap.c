/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 08:23:44 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 11:39:33 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

#define MINIMAP_SCALE  5
#define MINIMAP_SIZE   150
#define PLAYER_COLOR   0x00FF00 // Green color for player
#define WALL_COLOR     0x3D3D3D // Walls
#define FLOOR_COLOR    0x9FB6CD // Floors
#define MINIMAP_OFFSET_X  20
#define MINIMAP_OFFSET_Y  20

typedef struct	s_minimap_data
{
	int		block_size;
	int		minimap_tiles;
	float	player_tile_x;
	float	player_tile_y;
	float	start_x;
	float	start_y;
}			t_minimap_data;


static void	compute_minimap_data(t_map *map, t_game *game, t_minimap_data *data)
{
	data->block_size = TILE_SIZE / MINIMAP_SCALE;
	data->minimap_tiles = MINIMAP_SIZE / data->block_size;
	data->player_tile_x = game->player.x / TILE_SIZE;
	data->player_tile_y = game->player.y / TILE_SIZE;
	data->start_x = data->player_tile_x - data->minimap_tiles / 2.0f;
	data->start_y = data->player_tile_y - data->minimap_tiles / 2.0f;
	if (data->start_x < 0)
		data->start_x = 0;
	if (data->start_y < 0)
		data->start_y = 0;
	if (data->start_x + data->minimap_tiles > (int)map->columns)
		data->start_x = (int)map->columns - data->minimap_tiles;
	if (data->start_y + data->minimap_tiles > (int)map->rows)
		data->start_y = (int)map->rows - data->minimap_tiles;
}


void	draw_map_tiles(t_map *map, t_game *game, t_minimap_data *data)
{
	int	y;
	int	x;
	int	map_x;
	int	map_y;
	int	color;

	y = 0;
	while (y++ < data->minimap_tiles)
	{
		x = 0;
		while (x++ < data->minimap_tiles)
		{
			map_x = (int)(data->start_x + x);
			map_y = (int)(data->start_y + y);
			if (map_x >= 0 && map_x < (int)map->columns && map_y >= 0
				&& map_y < (int)map->rows)
			{
				color = FLOOR_COLOR;
				if (map->full_map[map_y][map_x] == '1')
					color = WALL_COLOR;
				draw_square((t_square){
					MINIMAP_OFFSET_X + x * data->block_size,
					MINIMAP_OFFSET_Y + y * data->block_size,
					data->block_size, color}, game);
			}
		}
	}
}

static void	draw_player_marker(t_game *game, t_minimap_data *data)
{
	int	player_x;
	int	player_y;

	player_x = MINIMAP_OFFSET_X + (data->player_tile_x - data->start_x)
		* data->block_size;
	player_y = MINIMAP_OFFSET_Y + (data->player_tile_y - data->start_y)
		* data->block_size;
	draw_square((t_square){player_x, player_y, data->block_size / 2,
		PLAYER_COLOR}, game);
}

void	draw_minimap(t_map *map, t_game *game)
{
	t_minimap_data	data;

	compute_minimap_data(map, game, &data);
	draw_map_tiles(map, game, &data);
	draw_player_marker(game, &data);
}
