/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/01/30 12:46:57 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->img_data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = color;
}

static void	clear_image(t_game *game)
{
	memset(game->img_data, 0, HEIGHT * game->size_line);
}

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

void	draw_cross(t_game *game)
{
	int	center_x;
	int	center_y;
	int	size;
	int	color;
	int	i;

	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	size = 10;
	color = 0x00FF00;
	i = -size;
	while (i <= size)
	{
		put_pixel(center_x + i, center_y, color, game);
		i++;
	}
	i = -size;
	while (i <= size)
	{
		put_pixel(center_x, center_y + i, color, game);
		i++;
	}
}

int	draw_loop(t_ctrl *ctrl)
{
	float	fov;
	float	start_angle;
	float	angle_step;
	double	delta_time;

	ctrl->anim.fc++;
	printf("DEBUG: ctrl->anim.fc: %zu\n", ctrl->anim.fc);
	if (ctrl->anim.fc >= TIME_SPEED)
	{
		ctrl->anim.fc = 0;
		ctrl->anim.ac++;
		printf("DEBUG: frame index: %zu\n", ctrl->anim.ac);
	}
	if (ctrl->anim.ac >= MAX_FRAMES)
	{
		ctrl->anim.ac = 0;
	}
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
	draw_cross(ctrl->game);
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
	return (0);
}
