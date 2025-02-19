/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 08:23:44 by stefan            #+#    #+#             */
/*   Updated: 2025/02/19 08:41:40 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

#define MINIMAP_SCALE  5
#define MINIMAP_SIZE   150
#define PLAYER_COLOR   0x00FF00 // Green color for player
#define WALL_COLOR     0x3D3D3D // Walls
#define FLOOR_COLOR    0x9FB6CD // Floors
#define ENEMY_COLOR    0xFF0000 // Enemy
#define CORPSE_COLOR 	0x808080 //Corpse
#define MINIMAP_OFFSET_X  20
#define MINIMAP_OFFSET_Y  20

t_enemy *get_enemy(size_t x, size_t y, t_map *map)
{
	size_t i = 0;
	
    while (i < map->enemies_counter)
    {
        if ((size_t)(map->enemies[i].x / TILE_SIZE) == x &&
            (size_t)(map->enemies[i].y / TILE_SIZE) == y)
        {
            // printf(COLOR_GREEN "[DEBUG] get_enemy() found enemy at (%zu, %zu), Dead: %d\n" COLOR_RESET, 
            //        x, y, map->enemies[i].is_dead);
            return &map->enemies[i];
        }
		i++;
    }
   // printf(COLOR_RED "[DEBUG] get_enemy() failed to find enemy at (%zu, %zu)\n" COLOR_RESET, x, y);
    return NULL;
}

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

// void	draw_map_tiles(t_map *map, t_game *game, t_minimap_data *data)
// {
// 	int	y;
// 	int	x;
// 	int	map_x;
// 	int	map_y;
// 	int	color;
// 	t_enemy	*enemy;

// 	enemy = NULL;
// 	y = 0;
// 	while (y++ < data->minimap_tiles)
// 	{
// 		x = 0;
// 		while (x++ < data->minimap_tiles)
// 		{
// 			map_x = (int)(data->start_x + x);
// 			map_y = (int)(data->start_y + y);
// 			if (map_x >= 0 && map_x < (int)map->columns && map_y >= 0
// 				&& map_y < (int)map->rows)
// 			{
// 				color = FLOOR_COLOR;
// 				if (map->full_map[map_y][map_x] == '1')
// 					color = WALL_COLOR;
// 				draw_square((t_square){
// 					MINIMAP_OFFSET_X + x * data->block_size,
// 					MINIMAP_OFFSET_Y + y * data->block_size,
// 					data->block_size, color}, game);
// 					if (map->full_map[map_y][map_x] == 'M' || map->full_map[map_y][map_x] == 'C')
// 					{
// 						enemy = get_enemy(map_x, map_y, map);
// 						if (enemy != NULL && enemy->is_dead)
// 						{
// 							//printf(COLOR_YELLOW "[DEBUG] Enemy at (%d, %d) is now a corpse.\n" COLOR_RESET, map_x, map_y);
// 							map->full_map[map_y][map_x] = 'C'; 
// 							color = CORPSE_COLOR;
// 						}
// 						else
// 						{
// 							color = ENEMY_COLOR;  
// 						}
// 					draw_square((t_square){
// 						MINIMAP_OFFSET_X + x * data->block_size
// 						+ data->block_size / 4,
// 						MINIMAP_OFFSET_Y + y * data->block_size
// 						+ data->block_size / 4,
// 						data->block_size / 2, color}, game);
// 				}
// 			}
// 		}
// 	}
// }

void	draw_map_tiles(t_map *map, t_game *game, t_minimap_data *data)
{
	int		x;
	int		y;
	int		map_x;
	int		map_y;
	int		color;
	t_enemy	*enemy;

	enemy = NULL;
	y = 0;
	while (y < data->minimap_tiles)
	{
		x = 0;
		while (x < data->minimap_tiles)
		{
			map_x = (int)(data->start_x + x);
			map_y = (int)(data->start_y + y);
			if (map_x >= 0 && map_x < (int)map->columns && map_y >= 0
				&& map_y < (int)map->rows)
			{
				color = FLOOR_COLOR;
				if (map->full_map[map_y][map_x] == '1')
					color = WALL_COLOR;
				draw_square((t_square)
				{
					MINIMAP_OFFSET_X + x * data->block_size,
					MINIMAP_OFFSET_Y + y * data->block_size,
					data->block_size, color}, game);
				if (map->full_map[map_y][map_x] == 'M'
						|| map->full_map[map_y][map_x] == 'C')
				{
					enemy = get_enemy(map_x, map_y, map);
					if (enemy != NULL && enemy->is_dead)
					{
						map->full_map[map_y][map_x] = 'C';
						color = CORPSE_COLOR;
					}
					else
						color = ENEMY_COLOR;
					draw_square((t_square)
					{
						MINIMAP_OFFSET_X + x * data->block_size
						+ data->block_size / 4,
						MINIMAP_OFFSET_Y + y * data->block_size
						+ data->block_size / 4,
						data->block_size / 2, color}, game);
				}
			}
			x++;
		}
		y++;
	}
}

void	draw_minimap_line(int x0, int y0, int x1, int y1, int color, t_game *game)
{
	float	dx;
	float	dy;
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;

	dx = x1 - x0;
	dy = y1 - y0;
	if (fabsf(dx) > fabsf(dy))
		steps = fabsf(dx);
	else
		steps = fabsf(dy);
	x_inc = dx / (float)steps;
	y_inc = dy / (float)steps;
	x = x0;
	y = y0;
	while (steps-- > 0)
	{
		put_pixel((int)x, (int)y, color, game);
		x += x_inc;
		y += y_inc;
	}
}

// Square with line
// static void	draw_player_marker(t_game *game, t_minimap_data *data)
// {
//     int	player_x;
//     int	player_y;

//     player_x = MINIMAP_OFFSET_X + (data->player_tile_x - data->start_x)
//         * data->block_size;
//     player_y = MINIMAP_OFFSET_Y + (data->player_tile_y - data->start_y)
//         * data->block_size;
//     draw_square((t_square){player_x, player_y, data->block_size / 2,
//         PLAYER_COLOR}, game);

//     // Draw direction line (assuming game->player.angle in radians)
//     int center_x = player_x + data->block_size / 4;
//     int center_y = player_y + data->block_size / 4;
//     int dir_x = center_x + cosf(game->player.angle) * (data->block_size / 1.2);
//     int dir_y = center_y + sinf(game->player.angle) * (data->block_size / 1.2);
//     draw_minimap_line(center_x, center_y, dir_x, dir_y, 0xFFFFFF, game);
// }

// Circle with line
static void	draw_circle(int center_x, int center_y, int radius, int color, t_game *game)
{
	int	y;
	int	x;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(center_x + x, center_y + y, color, game);
			x++;
		}
		y++;
	}
}

static void	draw_player_marker(t_game *game, t_minimap_data *data)
{
	int	center_x;
	int	center_y;
	int	radius;
	int	dir_x;
	int	dir_y;

	center_x = MINIMAP_OFFSET_X + (data->player_tile_x - data->start_x)
		* data->block_size + data->block_size / 2;
	center_y = MINIMAP_OFFSET_Y + (data->player_tile_y - data->start_y)
		* data->block_size + data->block_size / 2;
	radius = data->block_size / 4;
	draw_circle(center_x, center_y, radius, PLAYER_COLOR, game);
	dir_x = center_x + cosf(game->player.angle) * (data->block_size / 1.2);
	dir_y = center_y + sinf(game->player.angle) * (data->block_size / 1.2);
	draw_minimap_line(center_x, center_y, dir_x, dir_y, 0xFFFFFF, game);
}

static void	draw_minimap_frame(t_game *game, t_minimap_data *data)
{
	int	x;
	int	y;
	int	frame_color;
	int	minimap_width;
	int	minimap_height;

	frame_color = 0xA6A6A6;
	minimap_width = data->minimap_tiles * data->block_size;
	minimap_height = minimap_width;
	x = 0;
	while (x <= minimap_width)
	{
		put_pixel(MINIMAP_OFFSET_X + x, MINIMAP_OFFSET_Y, frame_color, game);
		x++;
	}
	x = 0;
	while (x <= minimap_width)
	{
		put_pixel(MINIMAP_OFFSET_X + x, MINIMAP_OFFSET_Y + minimap_height,
			frame_color, game);
		x++;
	}
	y = 0;
	while (y <= minimap_height)
	{
		put_pixel(MINIMAP_OFFSET_X, MINIMAP_OFFSET_Y + y, frame_color, game);
		y++;
	}
	y = 0;
	while (y <= minimap_height)
	{
		put_pixel(MINIMAP_OFFSET_X + minimap_width, MINIMAP_OFFSET_Y + y,
			frame_color, game);
		y++;
	}
}

void	draw_minimap(t_map *map, t_game *game)
{
	t_minimap_data	data;

	compute_minimap_data(map, game, &data);
	draw_map_tiles(map, game, &data);
	draw_player_marker(game, &data);
	draw_minimap_frame(game, &data);
}
