/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:19:57 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 16:45:10 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

int	mouse_click(int button, t_game *game)
{
	if (button == MOUSE_CLICK || button == SPACE)
		game->is_shooting = 1;
	return (0);
}

void	init_hooks(t_ctrl *ctrl)
{
	mlx_mouse_hide(ctrl->game->mlx, ctrl->game->win);
	mlx_mouse_move(ctrl->game->mlx, ctrl->game->win, WIDTH / 2, HEIGHT / 2);
	mlx_loop_hook(ctrl->game->mlx, draw_loop, ctrl);
	mlx_hook(ctrl->game->win, 6, 1L << 6, mouse_move, ctrl);
	mlx_hook(ctrl->game->win, 2, 1L << 0, key_press, ctrl);
	mlx_hook(ctrl->game->win, 3, 1L << 1, key_release, ctrl);
	mlx_mouse_hook(ctrl->game->win, mouse_click, ctrl->game);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
}

void	init_game_window(t_ctrl *ctrl)
{
	ctrl->game->mlx = mlx_init();
	if (!ctrl->game->mlx)
		clean_exit("Error: MLX initialization failed.\n", ctrl);
	ctrl->game->win = mlx_new_window(ctrl->game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!ctrl->game->win)
		clean_exit("Error: Window creation failed.\n", ctrl);
	ctrl->game->img = mlx_new_image(ctrl->game->mlx, WIDTH, HEIGHT);
	if (!ctrl->game->img)
	{
		mlx_destroy_window(ctrl->game->mlx, ctrl->game->win);
		clean_exit("Error: Image creation failed.\n", ctrl);
	}
	ctrl->game->img_data = mlx_get_data_addr(ctrl->game->img, &ctrl->game->bpp,
			&ctrl->game->size_line, &ctrl->game->endian);
	if (!ctrl->game->img_data)
	{
		mlx_destroy_image(ctrl->game->mlx, ctrl->game->img);
		mlx_destroy_window(ctrl->game->mlx, ctrl->game->win);
		clean_exit("Error: Image data not initialized.\n", ctrl);
	}
	ctrl->game->debug = 0;
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
}
