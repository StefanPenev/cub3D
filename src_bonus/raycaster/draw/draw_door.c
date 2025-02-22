/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:39:57 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 15:40:28 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Draws a door by applying texture mapping.
void	draw_door(t_ctrl *ctrl, t_raycast *rc, int col)
{
	rc->y = rc->draw_start;
	while (rc->y < rc->draw_end)
	{
		rc->tex_y = (int)rc->tex_pos & (TEX_HEIGHT - 1);
		rc->tex_pos += rc->step;
		rc->color = get_texture_color(&ctrl->game->door, rc->tex_x, rc->tex_y);
		put_pixel(col, rc->y, rc->color, ctrl->game);
		rc->y++;
	}
}
