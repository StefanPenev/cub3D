/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 15:28:36 by anilchen         ###   ########.fr       */
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

void	draw_weapon(t_texture *texture, t_game *game, int scale,
		int recoil_offset)
{
	int	color;
	int	screen_x;
	int	screen_y;
	int	y;
	int	x;
	int	weapon_x;
	int	weapon_y;
	int	i;
	int	j;

	weapon_x = (WIDTH / 2) - ((TEX_WIDTH * scale) / 2);
	// change divider to move weapon left or right
	weapon_y = HEIGHT - (TEX_HEIGHT * scale) + recoil_offset;
	y = 0;
	while (y < TEX_HEIGHT)
	{
		x = 0;
		while (x < TEX_WIDTH)
		{
			color = get_texture_color(texture, x, y);
			if (color != 0xFFFFFF)
			{
				i = 0;
				while (i < scale)
				{
					j = 0;
					while (j < scale)
					{
						screen_x = weapon_x + (x * scale) + i;
						screen_y = weapon_y + (y * scale) + j;
						if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0
							&& screen_y < HEIGHT)
							put_pixel(screen_x, screen_y, color, game);
						j++;
					}
					i++;
				}
			}
			x++;
		}
		y++;
	}
}

void	choose_weapon(t_game *game)
{
	int	scale;
	int	recoil_offset;

	recoil_offset = 0;
	scale = 3;
	if (game->is_shooting)
	{
		recoil_offset = 10;
		draw_weapon(&game->weapon_shoot, game, scale, recoil_offset);
		game->shoot_ac++;
		if (game->shoot_ac >= 50) //change this value to choose how long shooting frame should be on the screen
		{
			//recoil_offset = 0;
			game->is_shooting = 0;
			game->shoot_ac = 0;
		}
	}
	else
	{
		draw_weapon(&game->weapon_idle, game, scale, recoil_offset);
	}
}

int	draw_loop(t_ctrl *ctrl)
{
	float	fov;
	float	start_angle;
	float	angle_step;
	double	delta_time;
	size_t	i;

	i = 0;
	ctrl->anim.fc++;
	// printf("DEBUG: ctrl->anim.fc: %zu\n", ctrl->anim.fc);
	if (ctrl->anim.fc >= TIME_SPEED)
	{
		ctrl->anim.fc = 0;
		ctrl->anim.ac++;
		// printf("DEBUG: frame index: %zu\n", ctrl->anim.ac);
	}
	if (ctrl->anim.ac >= MAX_FRAMES)
	{
		ctrl->anim.ac = 0;
	}
	delta_time = compute_delta_time();
	// printf("DEBUG: delta_time = %f\n", delta_time);
	// door
	while (i < ctrl->map.doors_counter)
	{
		if (ctrl->map.doors[i].state == DOOR_OPENING)
		{
			update_doors(&ctrl->map.doors[i], &ctrl->map, delta_time);
		}
		i++;
	}
	// door
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
	// draw_doors(ctrl);
	draw_cross(ctrl->game);
	draw_minimap(&ctrl->map, ctrl->game);
	choose_weapon(ctrl->game);
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
	return (0);
}
