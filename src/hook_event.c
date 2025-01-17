/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_event.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:04:51 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 17:23:31 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	hook_esc(int keycode, t_ctrl *ctrl)
{
	if (keycode == KEY_ESC)
	{
		game_cleanup(ctrl);
		exit(0);
	}
	return (0);
}

// int	conditions(t_ctrl *ctrl, char tile)
// {
// 	if (tile == KEYS)
// 	{
// 		ctrl->game.map.keys--;
// 		ctrl->game.map.full[ctrl->player_tmp.y][ctrl->player_tmp.x] = FLOOR;
// 	}
// 	else if (tile == EXIT && ctrl->game.map.keys == 0)
// 	{
// 		ft_winner(ctrl);
// 		return (1);
// 	}
// 	else if (tile == ENEMY)
// 	{
// 		ft_loser(ctrl);
// 		return (1);
// 	}
// 	return (0);
// }

// void	move_viewpoint(t_ctrl *ctrl, int x_offset, int y_offset)
// {
// 	char	tile;

// 	ctrl->player_prev.x = ctrl->player.x;
// 	ctrl->player_prev.y = ctrl->player.y;
// 	ctrl->player_tmp.x = ctrl->player.x + x_offset;
// 	ctrl->player_tmp.y = ctrl->player.y + y_offset;
// 	if (ctrl->player_tmp.x >= 0 && ctrl->player_tmp.x < ctrl->game.map.columns
// 		&& ctrl->player_tmp.y >= 0 && ctrl->player_tmp.y < ctrl->game.map.rows)
// 	{
// 		tile = ctrl->game.map.full[ctrl->player_tmp.y][ctrl->player_tmp.x];
// 		if (tile != WALL)
// 		{
// 			if (conditions(ctrl, tile) == 0)
// 			{
// 				ctrl->player.x = ctrl->player_tmp.x;
// 				ctrl->player.y = ctrl->player_tmp.y;
// 				ctrl->game.moves_count++;
// 				ft_printf("Moves: %d\n", ctrl->game.moves_count);
// 				render_next_frame(ctrl);
// 			}
// 		}
// 	}
// }

// int	hook_keypress(int keycode, t_ctrl *ctrl)
// {
// 	if (keycode == KEY_W)
// 		move_viewpoint(ctrl, 0, -1);
// 	else if (keycode == KEY_S)
// 		move_viewpoint(ctrl, 0, 1);
// 	else if (keycode == KEY_A)
// 		move_viewpoint(ctrl, -1, 0);
// 	else if (keycode == KEY_D)
// 		move_viewpoint(ctrl, 1, 0);
// 	else if (keycode == KEY_ESC)
// 		hook_esc(keycode, ctrl);
// 	return (0);
// }
