/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:39:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 18:11:00 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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

void	choose_texture(t_raycast *rc, t_ctrl *ctrl)
{
	size_t		frame_index;
	t_texture	*sel_tex;

	sel_tex = NULL;
	if (rc->side == 0)
	{
		if (rc->ray_dir_x > 0)
			sel_tex = &ctrl->game->west_texture;
		else
			sel_tex = &ctrl->game->east_texture;
	}
	else
	{
		if (rc->ray_dir_y > 0)
			sel_tex = &ctrl->game->south_texture;
		else
			sel_tex = &ctrl->game->north_texture;
	}
	rc->selected_texture = sel_tex;
	frame_index = (ctrl->anim.ac * 2) % sel_tex->frames_count;
	if (!sel_tex->frames || !sel_tex->frames[frame_index])
	{
		printf("Error: Missing animation frame for texture\n");
		return ;
	}
	sel_tex->current_frame = frame_index;
	rc->selected_texture->addr = sel_tex->frames_addr[frame_index];
	rc->step = (float)TEX_HEIGHT / (float)rc->wall_height;
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

void	draw_skybox(t_game *gm)
{
	int		x;
	int		y;
	int		tex_x;
	int		tex_y;
	int		color;
	float	total_fov = M_PI;
	float	vertical_fov = M_PI / 3.0f;
    float	offset = gm->player.angle;
	float	sample_angle;
	float	v_ratio;
	float	vertical_angle;

	if (gm->ceiling_texture.frames_count <= 0)
	{
		draw_ceiling_with_colour(gm);
		return ;
	}
	y = 0;
	while (y < HEIGHT / 2)
	{
		v_ratio = (float)y / (HEIGHT / 2.0f);
		vertical_angle = (v_ratio - 0.5f) * vertical_fov;
		tex_y = (int)((vertical_angle + (vertical_fov / 2.0f)) / vertical_fov
				* gm->ceiling_texture.height) % gm->ceiling_texture.height;
		x = 0;
		while (x < WIDTH)
		{
			sample_angle = offset + ((float)x / (float)WIDTH) * total_fov;
			sample_angle = fmodf(sample_angle, 2.0f * M_PI);
			if (sample_angle < 0)
				sample_angle += 2.0f * M_PI;
			tex_x = (int)((sample_angle / (2.0f * M_PI))
					* gm->ceiling_texture.width) % gm->ceiling_texture.width;
			color = get_texture_color(&gm->ceiling_texture, tex_x, tex_y);
			put_pixel(x, y, color, gm);
			x++;
		}
		y++;
	}
}

void draw_floor(t_game *gm, t_raycast *rc, int col)
{
	int		y;
	float	row_dist;
	float	floor_x;
	float	floor_y;
	float	weight;
	float	scale_factor;
	int		tex_x;
	int		tex_y;
	int		color;

	if (gm->floor_texture.frames_count <= 0)
	{
		draw_floor_with_colour(gm, rc, col);
		return ;
	}
	scale_factor = 2.5f;
	y = rc->draw_end;
	while (y < HEIGHT)
	{
		row_dist = (0.5f * HEIGHT) / (y - HEIGHT / 2);
		weight = row_dist / rc->perp_dist;
		floor_x = weight * rc->hit_x + (1.0f - weight) * gm->player.x
			/ TILE_SIZE;
		floor_y = weight * rc->hit_y + (1.0f - weight) * gm->player.y
			/ TILE_SIZE;
		tex_x = (int)(floor_x * TEX_WIDTH * scale_factor) % TEX_WIDTH;
		tex_y = (int)(floor_y * TEX_HEIGHT * scale_factor) % TEX_HEIGHT;
		color = get_texture_color(&gm->floor_texture, tex_x, tex_y);
		put_pixel(col, y, color, gm);
		y++;
	}
}
