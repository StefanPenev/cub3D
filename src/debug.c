/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:22:45 by stefan            #+#    #+#             */
/*   Updated: 2025/01/22 11:28:06 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_line_coords(int x0, int y0, int x1, int y1, int color, t_game *game)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;

    while (1)
    {
        draw_square(x0, y0, 1, color, game);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void	draw_debug(t_ctrl *ctrl)
{
	t_game *game = ctrl->game;
    t_map *map = &ctrl->map;

    int block_width = WIDTH / map->columns;
    int block_height = HEIGHT / map->rows;
    int block_size = (block_width < block_height) ? block_width : block_height;
    for (size_t y = 0; y < map->rows; y++)
    {
        for (size_t x = 0; x < map->columns; x++)
        {
            int color = (map->full_map[y][x] == '1') ? 0xAAAAAA : 0x333333; // Wall or floor color
            draw_square(x * block_size, y * block_size, block_size, color, game);
        }
    }
    int player_x = (game->player.x / TILE_SIZE) * block_size;
    int player_y = (game->player.y / TILE_SIZE) * block_size;
    draw_square(player_x - block_size / 8, player_y - block_size / 8, block_size / 4, 0x00FF00, game);
    float fov = M_PI / 3.0f; // 60 degrees FOV
    float start_angle = game->player.angle - (fov / 2.0f);
    float angle_step = fov / WIDTH;
    for (int i = 0; i < WIDTH; i += 10) // Sparse rays for clarity
    {
        float ray_angle = start_angle + (i * angle_step);
        normalize_angle(&ray_angle);
        int hit = 0;
        float distance = 0.0f;
        size_t grid_x = (int)(game->player.x / TILE_SIZE);
        size_t grid_y = (int)(game->player.y / TILE_SIZE);
        float ray_dir_x = cosf(ray_angle);
        float ray_dir_y = sinf(ray_angle);
        float delta_dist_x = fabsf(1 / ray_dir_x);
        float delta_dist_y = fabsf(1 / ray_dir_y);

        int step_x = (ray_dir_x < 0) ? -1 : 1;
        int step_y = (ray_dir_y < 0) ? -1 : 1;

        float side_dist_x = (ray_dir_x < 0)
            ? (game->player.x / TILE_SIZE - grid_x) * delta_dist_x
            : (grid_x + 1.0f - game->player.x / TILE_SIZE) * delta_dist_x;
        float side_dist_y = (ray_dir_y < 0)
            ? (game->player.y / TILE_SIZE - grid_y) * delta_dist_y
            : (grid_y + 1.0f - game->player.y / TILE_SIZE) * delta_dist_y;

        while (!hit && (int)grid_x >= 0 && grid_x < map->columns && (int)grid_y >= 0 && grid_y < map->rows)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                grid_x += step_x;
            }
            else
            {
                side_dist_y += delta_dist_y;
                grid_y += step_y;
            }
            if (touch(grid_x, grid_y, map))
            {
                hit = 1;
                distance = (side_dist_x < side_dist_y) ? side_dist_x : side_dist_y;
            }
        }
        if (hit)
        {
            float hit_pos_x = game->player.x + ray_dir_x * distance * TILE_SIZE;
            float hit_pos_y = game->player.y + ray_dir_y * distance * TILE_SIZE;

            int hit_x = (hit_pos_x / TILE_SIZE) * block_size;
            int hit_y = (hit_pos_y / TILE_SIZE) * block_size;

            // Draw a line to visualize the ray
            draw_line_coords(player_x, player_y, hit_x, hit_y, 0x00FF00, game);
        }
    }
}