/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ceiling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 10:11:49 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 15:38:02 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "draw.h"

void	draw_ceiling_with_colour(t_game *gm)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT / 2)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, gm->ceiling_color, gm);
			x++;
		}
		y++;
	}
}

static void	skybox_draw_col(t_game *gm, int x, int y, t_skydraw sd)
{
	float	total_fov;
	float	sample_angle;
	int		tex_x;
	int		color;

	total_fov = M_PI;
	sample_angle = sd.offset + ((float)x / (float)WIDTH) * total_fov;
	sample_angle = fmodf(sample_angle, 2.0f * M_PI);
	if (sample_angle < 0)
		sample_angle += 2.0f * M_PI;
	tex_x = (int)((sample_angle / (2.0f * M_PI))
			* gm->ceiling_texture.width) % gm->ceiling_texture.width;
	color = get_texture_color(&gm->ceiling_texture, tex_x, sd.tex_y);
	put_pixel(x, y, color, gm);
}

static void	skybox_draw_cols(t_game *gm, int y, t_skydraw *sd)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		skybox_draw_col(gm, x, y, *sd);
		x++;
	}
}

static void	skybox_draw_row(t_game *gm, int y, float vfov, t_skydraw *sd)
{
	float	v_ratio;
	float	vertical_angle;

	v_ratio = (float)y / (HEIGHT / 2.0f);
	vertical_angle = (v_ratio - 0.5f) * vfov;
	sd->tex_y = (int)(((vertical_angle + (vfov / 2.0f)) / vfov)
			* gm->ceiling_texture.height) % gm->ceiling_texture.height;
	skybox_draw_cols(gm, y, sd);
}

void	draw_skybox(t_game *gm)
{
	t_skydraw	sd;
	float		vfov;
	int			y;

	if (gm->ceiling_texture.frames_count <= 0)
	{
		draw_ceiling_with_colour(gm);
		return ;
	}
	vfov = M_PI / 3.0f;
	sd.offset = gm->player.angle;
	y = 0;
	while (y < HEIGHT / 2)
	{
		skybox_draw_row(gm, y, vfov, &sd);
		y++;
	}
}
