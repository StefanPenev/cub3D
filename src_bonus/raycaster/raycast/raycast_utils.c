/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:39:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 12:09:10 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/**
 * compute_delta_time - Calculates the time elapsed since the last function call.
 *
 * This function retrieves the current time using `gettimeofday`, then computes
 * the difference between the current time and the last recorded time. This delta
 * time is used for frame timing and movement calculations in the game.
 *
 * Returns:
 * - The elapsed time in seconds as a double.
 */
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

/**
 * compute_wall_dimensions - Calculates the wall dimensions and draw range.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *  @pl: Pointer to the player structure containing player position.
 *
 * This function computes the distance to the wall (both real and
 * perpendicular), 
 * the wall height, and the vertical screen range (start and end) to draw the
 * wall 
 * based on the ray's intersection with the wall.
 * - `real_dist`: The true distance from the player to the wall.
 * - `perp_dist`: The perpendicular distance, used to compute wall height and
 * 	  avoid distortions.
 * - `wall_height`: The height of the wall on the screen based on its distance
 *    from the player.
 * - `draw_start` and `draw_end`: The vertical screen coordinates where the wall
 * 	  will be drawn.
 * - `hit_x` and `hit_y`: The exact point where the ray hits the wall in world
 *    coordinates.
 */
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

/**
 * compute_wall_x - Computes the exact texture coordinates for the wall hit.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *
 * This function calculates the X coordinate of the point where the ray hits
 * the wall, 
 * which is used to select the appropriate texture from the texture atlas.
 * It also 
 * calculates the texture X coordinate (`tex_x`) for sampling.
 * - `wall_x`: The precise X coordinate of the wall hit in world space.
 * - `tex_x`: The X coordinate in the texture, based on the wall's X hit.
 */
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

/**
 * select_wall_texture - Selects the appropriate wall texture based on the ray's
 * side and direction.
 * @rc: Pointer to the raycasting structure, which holds ray information.
 * @ctrl: Pointer to the control structure, which contains game-related settings.
 *
 * This function determines which texture to use for the wall, based on which
 * side of the map the
 * ray intersects and its direction. It returns a pointer to the corresponding
 * texture.
 */
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

/**
 * choose_texture - Chooses the correct texture for the wall and applies any
 * animation if present.
 * @rc: Pointer to the raycasting structure, which holds ray information and
 * the selected texture.
 * @ctrl: Pointer to the control structure, which contains the game's
 * animation data.
 *
 * This function selects the correct wall texture based on the ray's side and
 * direction, then checks
 * if the texture has frames for animation. It applies the correct animation
 * frame if available.
 */
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
