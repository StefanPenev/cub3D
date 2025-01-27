/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:12:05 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/27 15:34:39 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// void	select_frame(t_ctrl *ctrl)
// {
// 	ctrl->time.ac++;
// 	if (ctrl->time.ac >= 3)
// 		ctrl->time.ac = 0;
// 	if (ctrl->time.ac == 0)
//         mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
// 		0, 0);
// 		mlx_put_image_to_window(ctrl->mlx.mlx, ctrl->mlx.win,
// 			ctrl->img.img_player, ctrl->player.x * TILE_SIZE, ctrl->player.y
// 			* TILE_SIZE);
// 	else if (ctrl->time.ac == 1)
// 		mlx_put_image_to_window(ctrl->mlx.mlx, ctrl->mlx.win,
// 			ctrl->anim.img_player_frame2, ctrl->player.x * TILE_SIZE,
// 			ctrl->player.y * TILE_SIZE);
// 	else if (ctrl->time.ac == 2)
// 		mlx_put_image_to_window(ctrl->mlx.mlx, ctrl->mlx.win,
// 			ctrl->anim.img_player_frame3, ctrl->player.x * TILE_SIZE,
// 			ctrl->player.y * TILE_SIZE);
// }
