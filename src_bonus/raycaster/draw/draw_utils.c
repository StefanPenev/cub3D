/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:05:52 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 14:21:06 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

float	compute_distance(float x1, float y1, float x2, float y2)
{
	float	dx;
	float	dy;

	dx = x2 - x1;
	dy = y2 - y1;
	return (sqrtf(dx * dx + dy * dy));
}

void	clear_image(t_game *game)
{
	memset(game->img_data, 0, HEIGHT * game->size_line);
}

void	draw_square(t_square sq, t_game *game)
{
	int	i;
	int	j;
	int	x_pos;
	int	y_pos;

	j = 0;
	while (j < sq.size)
	{
		y_pos = sq.y + j;
		if (y_pos >= 0 && y_pos < HEIGHT)
		{
			i = 0;
			while (i < sq.size)
			{
				x_pos = sq.x + i;
				if (x_pos >= 0 && x_pos < WIDTH)
					put_pixel(x_pos, y_pos, sq.color, game);
				i++;
			}
		}
		j++;
	}
}

void	animate(t_ctrl *ctrl)
{
	ctrl->anim.fc++;
	if (ctrl->anim.fc >= TIME_SPEED)
	{
		ctrl->anim.fc = 0;
		ctrl->anim.ac++;
	}
	if (ctrl->anim.ac >= MAX_FRAMES)
		ctrl->anim.ac = 0;
}

void	animate_doors(t_ctrl *ctrl, double delta_time)
{
	size_t	i;

	i = 0;
	while (i < ctrl->map.doors_counter)
	{
		if (ctrl->map.doors[i].state == DOOR_OPENING
			|| ctrl->map.doors[i].state == DOOR_OPEN)
		{
			update_doors(&ctrl->map.doors[i], ctrl, delta_time);
		}
		i++;
	}
}
