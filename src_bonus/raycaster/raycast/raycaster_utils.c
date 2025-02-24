/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:57:32 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 12:05:40 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
 * put_pixel - Places a single pixel on the screen at the specified coordinates.
 * 	x: The x-coordinate of the pixel.
 * 	y: The y-coordinate of the pixel.
 * 	color: The color of the pixel in integer format.
 * 	game: Pointer to the game structure containing the image data.
 *
 * This function sets the color of a pixel at the specified (x, y) position on 
 * the screen. It checks if the coordinates are within the screen bounds and 
 * then writes the color value to the corresponding pixel in the image data
 * buffer.
 */
void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->img_data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = color;
}

/**
 * get_texture_color - Retrieves the color of a specific pixel in a texture.
 * 	@texture: Pointer to the texture structure.
 * 	@tex_x: The x-coordinate of the pixel in the texture.
 * 	@tex_y: The y-coordinate of the pixel in the texture.
 *
 * This function ensures that the requested pixel coordinates are within bounds
 * and then extracts the color value from the texture data.
 *
 * Returns:
 * - The integer color value of the specified texture pixel.
 */
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

/**
 * clamp - Restricts a value within a specified range.
 * 	@value: The input value to be clamped.
 * 	@min: The minimum allowed value.
 * 	@max: The maximum allowed value.
 *
 * This function ensures that the given value does not go below the minimum or
 * above the maximum limit.
 *
 * Returns:
 * - The clamped value within the range [min, max].
 */
int	clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

/**
 * touch - Checks if a grid cell contains a wall.
 * 	@grid_x: The x-coordinate of the grid cell.
 * 	@grid_y: The y-coordinate of the grid cell.
 * 	@map: Pointer to the map structure containing the game world.
 *
 * This function verifies whether the specified grid cell is within the map's
 * boundaries and checks if it contains a wall ('1').
 *
 * Returns:
 * - true if the cell contains a wall, false otherwise.
 */
bool	touch(size_t grid_x, size_t grid_y, t_map *map)
{
	if (grid_x >= map->columns || grid_y >= map->rows)
		return (false);
	return (map->full_map[grid_y][grid_x] == '1');
}

/**
 * normalize_angle - Adjusts an angle to be within the range [0, 2π].
 * 	@angle: Pointer to the angle variable to be normalized.
 *
 * This function ensures that the given angle stays within a valid range by
 * adding or subtracting multiples of 2π as necessary.
 */
void	normalize_angle(float *angle)
{
	while (*angle < 0)
		*angle += 2 * M_PI;
	while (*angle >= 2 * M_PI)
		*angle -= 2 * M_PI;
}
