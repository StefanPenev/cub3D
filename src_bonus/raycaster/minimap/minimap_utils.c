/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:41:30 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 18:03:19 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes_bonus/cub3d.h"
#include "minimap.h"

t_enemy	*get_enemy(size_t x, size_t y, t_map *map)
{
	size_t	i;

	i = 0;
	while (i < map->enemies_counter)
	{
		if ((size_t)(map->enemies[i].x / TILE_SIZE) == x
			&& (size_t)(map->enemies[i].y / TILE_SIZE) == y)
		{
			return (&map->enemies[i]);
		}
		i++;
	}
	return (NULL);
}

void	draw_minimap_edges(t_game *game, int end_x, int end_y)
{
	int	x;
	int	y;

	x = MINIMAP_OFFSET_X;
	y = MINIMAP_OFFSET_Y;
	while (x <= end_x)
	{
		put_pixel(x, MINIMAP_OFFSET_Y, FRAME_COLOR, game);
		put_pixel(x, end_y, FRAME_COLOR, game);
		x++;
	}
	while (y <= end_y)
	{
		put_pixel(MINIMAP_OFFSET_X, y, FRAME_COLOR, game);
		put_pixel(end_x, y, FRAME_COLOR, game);
		y++;
	}
}

void	draw_minimap_line(t_minimap_data *data, t_game *game)
{
	float	dx;
	float	dy;
	int		steps;
	float	x_inc;
	float	y_inc;

	dx = data->x1 - data->x0;
	dy = data->y1 - data->y0;
	if (fabsf(dx) > fabsf(dy))
		steps = fabsf(dx);
	else
		steps = fabsf(dy);
	x_inc = dx / (float)steps;
	y_inc = dy / (float)steps;
	while (steps-- > 0)
	{
		put_pixel(data->x0, data->y0, PLAYER_COLOR, game);
		data->x0 += x_inc;
		data->y0 += y_inc;
	}
}

void	draw_circle(int center_x, int center_y, int radius, t_game *game)
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
				put_pixel(center_x + x, center_y + y, PLAYER_COLOR, game);
			x++;
		}
		y++;
	}
}
