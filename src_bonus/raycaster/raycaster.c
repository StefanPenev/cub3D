/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:43:51 by stefan            #+#    #+#             */
/*   Updated: 2025/01/30 10:22:45 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	draw_line(t_ctrl *ctrl, float ray_angle, int screen_column)
{
	float		ray_dir_x;
	float		ray_dir_y;
	int			mapX;
	int			mapY;
	float		delta_dist_x;
	float		delta_dist_y;
	int			hit;
	int			side;
	float		real_dist;
	float		perp_dist;
	int			wall_height;
	int			draw_start;
	int			draw_end;
	float		hitX;
	float		hitY;
	float		wall_x;
	int			tex_x;
	t_texture	*selected_texture;
	float		step;
	float		tex_pos;
	int			tex_y;
	int			color;
	size_t			frame_index;
	void		*current_frame;

	frame_index = ctrl->anim.ac;
	normalize_angle(&ray_angle);
	ray_dir_x = cosf(ray_angle);
	ray_dir_y = sinf(ray_angle);
	mapX = (int)(ctrl->game->player.x / TILE_SIZE);
	mapY = (int)(ctrl->game->player.y / TILE_SIZE);
	delta_dist_x = (ray_dir_x == 0) ? 1e30f : fabsf(1.0f / ray_dir_x);
	delta_dist_y = (ray_dir_y == 0) ? 1e30f : fabsf(1.0f / ray_dir_y);
	int step_x, step_y;
	float side_dist_x, side_dist_y;
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (ctrl->game->player.x / TILE_SIZE - mapX) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (mapX + 1.0f - ctrl->game->player.x / TILE_SIZE)
			* delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (ctrl->game->player.y / TILE_SIZE - mapY) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (mapY + 1.0f - ctrl->game->player.y / TILE_SIZE)
			* delta_dist_y;
	}
	hit = 0;
	side = 0;
	while (!hit)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			mapX += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			mapY += step_y;
			side = 1;
		}
		if (mapX < 0 || (size_t)mapX >= ctrl->map.columns || mapY < 0
			|| (size_t)mapY >= ctrl->map.rows)
			break ;
		if (touch(mapX, mapY, &ctrl->map))
			hit = 1;
	}
	real_dist = 1e30f;
	perp_dist = 1e30f;
	if (hit)
	{
		if (side == 0)
			real_dist = (side_dist_x - delta_dist_x);
		else
			real_dist = (side_dist_y - delta_dist_y);
		perp_dist = real_dist;
		if (perp_dist == 0)
			perp_dist = 1e-30f;
	}
	wall_height = (int)(HEIGHT / perp_dist);
	draw_start = clamp(-wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
	draw_end = clamp(wall_height / 2 + HEIGHT / 2, 0, HEIGHT - 1);
	hitX = ctrl->game->player.x / TILE_SIZE + real_dist * ray_dir_x;
	hitY = ctrl->game->player.y / TILE_SIZE + real_dist * ray_dir_y;
	wall_x = (side == 0) ? hitY : hitX;
	wall_x -= floorf(wall_x);
	tex_x = (int)(wall_x * (float)TEX_WIDTH);
	if ((side == 0 && ray_dir_x > 0) || (side == 1 && ray_dir_y < 0))
		tex_x = TEX_WIDTH - tex_x - 1;
	tex_x = clamp(tex_x, 0, TEX_WIDTH - 1);
	if (side == 0)
	{
		if (ray_dir_x > 0)
			selected_texture = &ctrl->game->west_texture;
		else
			selected_texture = &ctrl->game->east_texture;
	}
	else
	{
		if (ray_dir_y > 0)
			selected_texture = &ctrl->game->south_texture;
		else
			selected_texture = &ctrl->game->north_texture;
	}
	step = 1.0f * TEX_HEIGHT / wall_height;
	tex_pos = (draw_start - HEIGHT / 2 + wall_height / 2) * step;
	for (int y = draw_start; y < draw_end; y++)
	{
		tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
		tex_pos += step;
		current_frame = selected_texture->frames[frame_index];
		if (!current_frame)
		{
			printf("Error: failed to load texture frame\n");
			return ;
		}
		selected_texture->addr = mlx_get_data_addr(current_frame,
				&selected_texture->bits_per_pixel,
				&selected_texture->line_length, &selected_texture->endian);
		color = get_texture_color(selected_texture, tex_x, tex_y);
		put_pixel(screen_column, y, color, ctrl->game);
	}
	for (int y = 0; y < draw_start; y++)
		put_pixel(screen_column, y, ctrl->game->ceiling_color, ctrl->game);
	for (int y = draw_end; y < HEIGHT; y++)
		put_pixel(screen_column, y, ctrl->game->floor_color, ctrl->game);
}

void	handle_rays(t_ctrl *ctrl, float start_angle, float angle_step)
{
	int		i;
	float	ray_angle;

	i = 0;
	while (i < WIDTH)
	{
		ray_angle = start_angle + (i * angle_step);
		if (ray_angle < 0)
			ray_angle += 2.0f * M_PI;
		else if (ray_angle >= 2.0f * M_PI)
			ray_angle -= 2.0f * M_PI;
		// draw_line(&ctrl->game->player, ctrl->game, &ctrl->map, ray_angle, i);
		draw_line(ctrl, ray_angle, i);
		i++;
	}
}
