/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:57:32 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:25:49 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * compute_delta_time - Calculates the time elapsed since the last function call.
 *
 * This function retrieves the current time using `gettimeofday`, then computes
 * the difference between the current time and the last recorded time. This delta
 * time is used for frame timing and movement calculations in the game.
 *
 * Returns:
 * - The elapsed time in seconds as a double.
 */
double	compute_delta_time(void)
{
	struct timeval			current_time;
	double					delta_time;
	static struct timeval	last_time = {0};

	gettimeofday(&current_time, NULL);
	if (last_time.tv_sec == 0 && last_time.tv_usec == 0)
		last_time = current_time;
	delta_time = (double)(current_time.tv_sec - last_time.tv_sec);
	delta_time += (double)(current_time.tv_usec
			- last_time.tv_usec) / 1000000.0;
	last_time = current_time;
	return (delta_time);
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

	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= TEX_WIDTH)
		tex_x = TEX_WIDTH - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= TEX_HEIGHT)
		tex_y = TEX_HEIGHT - 1;
	pixel = texture->addr + (tex_y * texture->line_length
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
