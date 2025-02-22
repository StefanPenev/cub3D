/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:57:32 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 15:42:42 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->img_data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = color;
}

int	get_texture_color(t_texture *texture, int tex_x, int tex_y)
{
	char	*pixel;
	int		frame;

	frame = texture->current_frame;
	if (texture->frames_count <= 0)
		return (0x000000);
	if (frame < 0 || frame >= texture->frames_count)
		frame = 0;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	pixel = texture->frames_addr[frame] + (tex_y * texture->line_length
			+ tex_x * (texture->bits_per_pixel / 8));
	return (*(int *)pixel);
}

int	clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

bool	touch(size_t grid_x, size_t grid_y, t_map *map)
{
	if (grid_x >= map->columns || grid_y >= map->rows)
		return (false);
	return (map->full_map[grid_y][grid_x] == '1');
}

void	normalize_angle(float *angle)
{
	while (*angle < 0)
		*angle += 2 * M_PI;
	while (*angle >= 2 * M_PI)
		*angle -= 2 * M_PI;
}
