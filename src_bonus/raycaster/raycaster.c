/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:51 by stefan            #+#    #+#             */
/*   Updated: 2025/01/27 14:41:26 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

float	distance_(float x, float y)
{
	return (sqrtf(x * x + y * y));
}

float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
	float	delta_x;
	float	delta_y;
	float	angle;
	float	fix_dist;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	angle = atan2f(delta_y, delta_x) - game->player.angle;
	fix_dist = distance_(delta_x, delta_y) * cosf(angle);
	return (fix_dist);
}

void	draw_line(t_player *player, t_game *game, t_map *map,
    float ray_angle, int screen_column)
{
    normalize_angle(&ray_angle);
    float ray_dir_x = cosf(ray_angle);
    float ray_dir_y = sinf(ray_angle);
    int mapX = (int)(player->x / TILE_SIZE);
    int mapY = (int)(player->y / TILE_SIZE);
    float delta_dist_x = (ray_dir_x == 0) ? 1e30f : fabsf(1.0f / ray_dir_x);
    float delta_dist_y = (ray_dir_y == 0) ? 1e30f : fabsf(1.0f / ray_dir_y);
    int step_x, step_y;
    float side_dist_x, side_dist_y;

    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (player->x / TILE_SIZE - mapX) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (mapX + 1.0f - player->x / TILE_SIZE) * delta_dist_x;
    }
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (player->y / TILE_SIZE - mapY) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (mapY + 1.0f - player->y / TILE_SIZE) * delta_dist_y;
    }

    int hit = 0;
    int side = 0;
    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            mapX += step_x;
            side = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            mapY += step_y;
            side = 1;
        }
        if (mapX < 0 || (size_t)mapX >= map->columns || mapY < 0 || (size_t)mapY >= map->rows)
            break;
        if (touch(mapX, mapY, map))
            hit = 1;
    }

    float real_dist = 1e30f;
    float perp_dist = 1e30f;
    if (hit)
    {
        if (side == 0)
            real_dist = (side_dist_x - delta_dist_x);
        else
            real_dist = (side_dist_y - delta_dist_y);
        perp_dist = real_dist;
        if (perp_dist == 0) perp_dist = 1e-30f;
    }

    int wall_height = (int)(HEIGHT / perp_dist);
    int draw_start = clamp(-wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
    int draw_end = clamp(wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
    float hitX = player->x / TILE_SIZE + real_dist * ray_dir_x;
    float hitY = player->y / TILE_SIZE + real_dist * ray_dir_y;
    float wall_x = (side == 0) ? hitY : hitX;
    wall_x -= floorf(wall_x);

    int tex_x = (int)(wall_x * (float)TEX_WIDTH);
    if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
        tex_x = TEX_WIDTH - tex_x - 1;
    tex_x = clamp(tex_x, 0, TEX_WIDTH - 1);

    t_texture *selected_texture;
    if (side == 0)
    {
        if (ray_dir_x > 0)
            selected_texture = &game->west_texture;
        else
            selected_texture = &game->east_texture;
    }
    else
    {
        if (ray_dir_y > 0)
            selected_texture = &game->south_texture;
        else
            selected_texture = &game->north_texture;
    }

    float step = 1.0f * TEX_HEIGHT / wall_height;
    float tex_pos = (draw_start - HEIGHT / 2 + wall_height / 2) * step;
    for (int y = draw_start; y < draw_end; y++)
    {
        int tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
        tex_pos += step;
        int color = get_texture_color(selected_texture, tex_x, tex_y);
        put_pixel(screen_column, y, color, game);
    }

    for (int y = 0; y < draw_start; y++)
        put_pixel(screen_column, y, game->ceiling_color, game);
    for (int y = draw_end; y < HEIGHT; y++)
        put_pixel(screen_column, y, game->floor_color, game);
}

void	handle_rays(t_ctrl *ctrl, float start_angle, float angle_step)
{
	int		i;
	float	ray_angle;

	i = 0;
	while (i < WIDTH)
	{
		ray_angle = start_angle + (i * angle_step);
		if (ray_angle < 0)
			ray_angle += 2.0f * M_PI;
		else if (ray_angle >= 2.0f * M_PI)
			ray_angle -= 2.0f * M_PI;
		draw_line(&ctrl->game->player, ctrl->game, &ctrl->map, ray_angle, i);
		i++;
	}
}
