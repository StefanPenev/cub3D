/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:02:24 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 14:40:33 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "draw.h"

void	draw_crosshair(t_game *game)
{
	int		x;
	int		y;
	int		i;
	int		j;
	size_t	color;

	if (!game)
		return ;
	x = (WIDTH / 2) - (game->crosshair.width / 2);
	y = (HEIGHT / 2) - (game->crosshair.height / 2);
	i = 0;
	while (i < game->crosshair.height)
	{
		j = 0;
		while (j < game->crosshair.width)
		{
			color = get_texture_color(&game->crosshair, j, i);
			if ((color & 0xFFFFFF) != 0x000000)
			{
				put_pixel((x + 9) + j, (y + 35) + i, color, game);
			}
			j++;
		}
		i++;
	}
}

static void	scale_pixel_block(t_game *game, int color, t_weapon weapon)
{
	int	i;
	int	j;
	int	screen_x;
	int	screen_y;

	i = 0;
	while (i < weapon.scale)
	{
		j = 0;
		while (j < weapon.scale)
		{
			screen_x = weapon.base_x + i;
			screen_y = weapon.base_y + j;
			if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0
				&& screen_y < HEIGHT)
				put_pixel(screen_x, screen_y, color, game);
			j++;
		}
		i++;
	}
}

static void	draw_weapon_line(t_texture *texture, t_game *game,
	t_weapon weapon, int y)
{
	int	x;
	int	color;

	x = 0;
	while (x < TEX_WIDTH)
	{
		color = get_texture_color(texture, x, y);
		if ((color & 0xFFFFFF) != 0x000000)
		{
			weapon.base_x = weapon.offset_x + (x * weapon.scale);
			weapon.base_y = weapon.offset_y + (y * weapon.scale);
			scale_pixel_block(game, color, weapon);
		}
		x++;
	}
}

static void	draw_weapon(t_texture *texture, t_game *game, int scale,
	int recoil_offset)
{
	t_weapon	weapon;
	int			y;

	weapon.offset_x = (WIDTH / 2) - ((TEX_WIDTH * scale) / 2.3);
	weapon.offset_y = (HEIGHT - (TEX_HEIGHT * scale)) + recoil_offset;
	weapon.scale = scale;
	y = 0;
	while (y < TEX_HEIGHT)
	{
		draw_weapon_line(texture, game, weapon, y);
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
		if (game->shoot_ac >= 50)
		{
			game->is_shooting = 0;
			game->shoot_ac = 0;
		}
	}
	else
		draw_weapon(&game->weapon_idle, game, scale, recoil_offset);
}
