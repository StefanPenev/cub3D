/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 10:08:08 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 14:47:22 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "draw.h"

void	draw_floor_with_colour(t_game *gm, t_raycast *rc, int col)
{
	int	y;

	y = rc->draw_end;
	while (y < HEIGHT)
	{
		put_pixel(col, y, gm->floor_color, gm);
		y++;
	}
}

static void	calc_floor_info(t_floorcalc *fc, t_game *gm, t_raycast *rc, int y)
{
	float	scale;

	scale = 2.5f;
	fc->row_dist = (0.5f * HEIGHT) / (y - (HEIGHT / 2));
	fc->weight = fc->row_dist / rc->perp_dist;
	fc->floor_x = fc->weight * rc->hit_x
		+ (1.0f - fc->weight) * (gm->player.x / TILE_SIZE);
	fc->floor_y = fc->weight * rc->hit_y
		+ (1.0f - fc->weight) * (gm->player.y / TILE_SIZE);
	fc->tex_x = (int)(fc->floor_x * TEX_WIDTH * scale) % TEX_WIDTH;
	fc->tex_y = (int)(fc->floor_y * TEX_HEIGHT * scale) % TEX_HEIGHT;
	fc->color = get_texture_color(&gm->floor_texture, fc->tex_x, fc->tex_y);
}

void	draw_floor(t_game *gm, t_raycast *rc, int col)
{
	int			y;
	t_floorcalc	fc;

	if (gm->floor_texture.frames_count <= 0)
	{
		draw_floor_with_colour(gm, rc, col);
		return ;
	}
	y = rc->draw_end;
	while (y < HEIGHT)
	{
		calc_floor_info(&fc, gm, rc, y);
		put_pixel(col, y, fc.color, gm);
		y++;
	}
}
