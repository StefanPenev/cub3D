/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 10:11:31 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 18:31:54 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "draw.h"

static bool	enemy_wall_check(t_ctrl *ctrl, t_player *pl, t_enemy_draw *ed)
{
	float	rx;
	float	ry;
	float	sx;
	float	sy;
	float	rd;

	rx = pl->x;
	ry = pl->y;
	sx = (ed->dx / ed->dist) * (TILE_SIZE / 16.0f);
	sy = (ed->dy / ed->dist) * (TILE_SIZE / 16.0f);
	rd = 0;
	while (rd < ed->dist)
	{
		rx += sx;
		ry += sy;
		rd += TILE_SIZE / 16.0f;
		if (is_wall(ctrl, rx, ry))
			return (true);
	}
	return (false);
}

static bool	enemy_zbuffer_check(t_ctrl *ctrl, t_enemy_draw *ed)
{
	int	mc;

	mc = ed->sx + (ed->w / 2);
	if (mc >= 0 && mc < WIDTH && ctrl->game->zbuffer)
	{
		if (ed->eff_dist > ctrl->game->zbuffer[mc])
			return (true);
	}
	return (false);
}

static void	enemy_draw_sprite_column(t_ctrl *ctrl, t_enemy_draw *ed, int column)
{
	int	screen_x;
	int	screen_y;
	int	texture_x;
	int	color;
	int	y;

	screen_x = ed->sx + column;
	texture_x = column * TEX_WIDTH / ed->w;
	y = 0;
	while (y < ed->h)
	{
		screen_y = ed->sy + y;
		if (screen_y >= 0 && screen_y < HEIGHT)
		{
			color = get_texture_color(&ctrl->game->enemy, texture_x,
					(y * TEX_HEIGHT / ed->h));
			if (color != 0)
				put_pixel(screen_x, screen_y, color, ctrl->game);
		}
		y++;
	}
}

static void	enemy_draw_sprite(t_ctrl *ctrl, t_enemy_draw *ed)
{
	int	column;
	int	screen_x;

	column = 0;
	while (column < ed->w)
	{
		screen_x = ed->sx + column;
		if (screen_x >= 0 && screen_x < WIDTH)
			enemy_draw_sprite_column(ctrl, ed, column);
		column++;
	}
}

void	draw_enemy(t_ctrl *ctrl, t_player *pl, t_enemy *en)
{
	t_enemy_draw	ed;

	if (enemy_is_dead(en))
		return ;
	enemy_fill_data(&ed, pl, en);
	if (!enemy_in_fov(&ed, pl->angle))
		return ;
	ed.fov = M_PI / 3.0f;
	ed.dist_plane = (WIDTH / 2.0f) / tanf(ed.fov / 2.0f);
	ed.h = abs((int)((TILE_SIZE / ed.eff_dist) * ed.dist_plane));
	ed.w = ed.h;
	ed.sx = (int)((WIDTH / 2.0f) + (ed.diff / (ed.fov / 2.0f))
			* (WIDTH / 2.0f)) - (ed.w / 2);
	ed.sy = (HEIGHT / 2) - (ed.h / 2);
	if (enemy_wall_check(ctrl, pl, &ed))
		return ;
	if (enemy_zbuffer_check(ctrl, &ed))
		return ;
	enemy_draw_sprite(ctrl, &ed);
}
