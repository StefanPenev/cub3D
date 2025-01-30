/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:25:11 by stefan            #+#    #+#             */
/*   Updated: 2025/01/30 10:27:03 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	compute_wall_dimensions(t_raycast *rc, t_player *pl)
{
	if (rc->hit)
	{
		if (rc->side == 0)
			rc->real_dist = rc->side_dist_x - rc->delta_dist_x;
		else
			rc->real_dist = rc->side_dist_y - rc->delta_dist_y;
		rc->perp_dist = rc->real_dist;
		if (rc->perp_dist == 0.0f)
			rc->perp_dist = 1e-30f;
	}
	rc->wall_height = (int)(HEIGHT / rc->perp_dist);
	rc->draw_start = clamp(-rc->wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
	rc->draw_end = clamp(rc->wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
	rc->hit_x = pl->x / TILE_SIZE + rc->real_dist * rc->ray_dir_x;
	rc->hit_y = pl->y / TILE_SIZE + rc->real_dist * rc->ray_dir_y;
}

void	compute_wall_x(t_raycast *rc)
{
	if (rc->side == 0)
		rc->wall_x = rc->hit_y;
	else
		rc->wall_x = rc->hit_x;
	rc->wall_x -= floorf(rc->wall_x);
	rc->tex_x = (int)(rc->wall_x * (float)TEX_WIDTH);
	if ((rc->side == 0 && rc->ray_dir_x > 0) || (rc->side == 1
			&& rc->ray_dir_y < 0))
		rc->tex_x = TEX_WIDTH - rc->tex_x - 1;
	rc->tex_x = clamp(rc->tex_x, 0, TEX_WIDTH - 1);
}

void	choose_texture(t_raycast *rc, t_game *gm)
{
	if (rc->side == 0)
	{
		if (rc->ray_dir_x > 0)
			rc->selected_texture = &gm->west_texture;
		else
			rc->selected_texture = &gm->east_texture;
	}
	else
	{
		if (rc->ray_dir_y > 0)
			rc->selected_texture = &gm->south_texture;
		else
			rc->selected_texture = &gm->north_texture;
	}
	rc->step = 1.0f * TEX_HEIGHT / rc->wall_height;
	rc->tex_pos = (rc->draw_start - HEIGHT / 2 + rc->wall_height / 2)
		* rc->step;
}

void	draw_wall(t_game *gm, t_raycast *rc, int col)
{
	rc->y = rc->draw_start;
	while (rc->y < rc->draw_end)
	{
		rc->tex_y = (int)rc->tex_pos & (TEX_HEIGHT - 1);
		rc->tex_pos += rc->step;
		rc->color = get_texture_color(rc->selected_texture, rc->tex_x,
				rc->tex_y);
		put_pixel(col, rc->y, rc->color, gm);
		rc->y++;
	}
}

void	draw_ceil_floor(t_game *gm, t_raycast *rc, int col)
{
	rc->y = 0;
	while (rc->y < rc->draw_start)
	{
		put_pixel(col, rc->y, gm->ceiling_color, gm);
		rc->y++;
	}
	rc->y = rc->draw_end;
	while (rc->y < HEIGHT)
	{
		put_pixel(col, rc->y, gm->floor_color, gm);
		rc->y++;
	}
}
