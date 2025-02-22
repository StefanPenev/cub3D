/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:41:06 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 18:32:15 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
