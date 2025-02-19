/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:12:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/19 16:07:00 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Handles mouse movement events to rotate the player's view angle.
// - Tracks the horizontal movement of the mouse and adjusts
// the player's angle accordingly.
// - Ensures the player's angle value remains within the
// range [0, 2π] for consistency.

int	mouse_move(int x, int y, t_ctrl *ctrl)
{
	static int	prev_x = -1;
	int			dx;
	t_player	*player;

	player = &ctrl->game->player;
	if (prev_x < 0)
		prev_x = x;
	dx = x - prev_x;
	player->angle += dx * 0.010f;
	if (player->angle > 2 * M_PI)
		player->angle -= 2 * M_PI;
	else if (player->angle < 0)
		player->angle += 2 * M_PI;
	prev_x = x;
	(void)y;
	return (0);
}
