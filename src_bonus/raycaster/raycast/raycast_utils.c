/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:39:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 15:46:24 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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

static t_texture	*select_wall_texture(t_raycast *rc, t_ctrl *ctrl)
{
	if (rc->side == 0)
	{
		if (rc->ray_dir_x > 0)
			return (&ctrl->game->west_texture);
		else
			return (&ctrl->game->east_texture);
	}
	else
	{
		if (rc->ray_dir_y > 0)
			return (&ctrl->game->south_texture);
		else
			return (&ctrl->game->north_texture);
	}
}

void	choose_texture(t_raycast *rc, t_ctrl *ctrl)
{
	size_t		frame_index;
	t_texture	*sel_tex;

	sel_tex = select_wall_texture(rc, ctrl);
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
