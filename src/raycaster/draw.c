/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:06:56 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
 * clear_image - Clears the image buffer by setting all pixels to black (0).
 * 	game: Pointer to the game structure containing the image data.
 *
 * This function resets the image buffer by filling it with zeros, effectively
 * clearing the screen for the next frame to be drawn.
 */
static void	clear_image(t_game *game)
{
	memset(game->img_data, 0, HEIGHT * game->size_line);
}

/**
 * draw_square - Draws a square on the screen at the specified position.
 * 	sq: The square structure containing the position, size, and color of the
 *  square.
 * 	game: Pointer to the game structure.
 *
 * This function loops through the pixels of the square, drawing them one by one 
 * using the `put_pixel` function. It ensures that the square's pixels stay
 * within 
 * the screen boundaries and renders the square at the specified position with 
 * the given color.
 */
void	draw_square(t_square sq, t_game *game)
{
	int	i;
	int	j;
	int	x_pos;
	int	y_pos;

	j = 0;
	while (j < sq.size)
	{
		y_pos = sq.y + j;
		if (y_pos >= 0 && y_pos < HEIGHT)
		{
			i = 0;
			while (i < sq.size)
			{
				x_pos = sq.x + i;
				if (x_pos >= 0 && x_pos < WIDTH)
					put_pixel(x_pos, y_pos, sq.color, game);
				i++;
			}
		}
		j++;
	}
}

/*
 * draw_map - Draws the entire map on the screen.
 * 	mapp: Pointer to the map structure containing the full map data.
 * 	game: Pointer to the game structure.
 *
 * This function iterates through the map's grid and draws each wall ('1') as a 
 * square on the screen. It calls `draw_square` to render each wall tile at the 
 * corresponding position on the screen.
 */
void	draw_map(t_map *mapp, t_game *game)
{
	int		x;
	int		y;
	char	**map;
	int		block_size;
	int		color;

	color = MAP_COLOR;
	block_size = TILE_SIZE;
	map = mapp->full_map;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
			{
				draw_square(init_square(x * block_size, y * block_size,
						block_size, color), game);
			}
			x++;
		}
		y++;
	}
}

/**
 * draw_loop - The main game loop that updates and draws each frame.
 * 	ctrl: Pointer to the game control structure containing player and game data.
 *
 * This function is called each frame to update the game state:
 * - It computes the time elapsed since the last frame using
 * 	`compute_delta_time`.
 * - Clears the image buffer using `clear_image`.
 * - Moves the player based on input and the elapsed time.
 * - If debugging is enabled, it draws the debug information. Otherwise, it
 * 	 handles 
 *   raycasting to render the game world.
 * - Finally, it displays the updated image in the window using
 *   `mlx_put_image_to_window`.
 */
int	draw_loop(t_ctrl *ctrl)
{
	float	fov;
	float	start_angle;
	float	angle_step;
	double	delta_time;

	delta_time = compute_delta_time();
	clear_image(ctrl->game);
	move_player(ctrl, delta_time);
	if (ctrl->game->debug)
		draw_debug(ctrl);
	else
	{
		fov = M_PI / 3.0f;
		start_angle = ctrl->game->player.angle - (fov / 2.0f);
		angle_step = fov / WIDTH;
		handle_rays(ctrl, start_angle, angle_step);
	}
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
	return (0);
}
