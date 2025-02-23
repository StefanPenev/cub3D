/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:25:11 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:24:54 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
 * choose_texture - Selects the appropriate texture for the wall based on
 * the ray's side.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *  @gm: Pointer to the game structure containing the textures.
 *
 * This function selects the texture to use for the wall depending on the
 * side the ray hit:
 * - If the ray hit a vertical wall (`side == 0`), it uses the east or
 * west texture.
 * - If the ray hit a horizontal wall (`side == 1`), it uses the north
 * or south texture.
 * It also calculates the texture step size and initializes the texture
 * position for the vertical slice.
 */
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

/**
 * draw_wall - Draws the wall slice on the screen for a given column.
 *  @gm: Pointer to the game structure.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *  @col: The column index on the screen where the wall is drawn.
 *
 * This function loops through the vertical range of the wall (`draw_start`
 * to `draw_end`)
 * and draws the texture on the screen for the current column. It fetches
 * the pixel color 
 * from the selected texture and places it at the appropriate screen coordinates.
 */
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

/**
 * draw_ceil_floor - Draws the ceiling and floor for a given column.
 *  @gm: Pointer to the game structure.
 *  @rc: Pointer to the raycasting structure containing ray data.
 *  @col: The column index on the screen where the ceiling and floor are drawn.
 *
 * This function draws the ceiling above the wall and the floor below the wall 
 * for the given column. The ceiling and floor colors are fetched from the
 * game structure.
 */
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
