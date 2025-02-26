/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:19:57 by stefan            #+#    #+#             */
/*   Updated: 2025/02/26 12:00:42 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_hooks - Initializes the event hooks for the game.
 * @ctrl: The control structure containing the game data.
 * 
 * Sets up hooks for the main loop, key press/release, and window close events.
 */
void	init_hooks(t_ctrl *ctrl)
{
	mlx_loop_hook(ctrl->game->mlx, draw_loop, ctrl);
	mlx_hook(ctrl->game->win, 2, 1L << 0, key_press, ctrl);
	mlx_hook(ctrl->game->win, 3, 1L << 1, key_release, ctrl);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
}

/**
 * init_game_window - Initializes the game window and image.
 * @ctrl: The control structure containing the game data.
 * 
 * Creates the MLX window, image, and retrieves image data. Exits if any
 * initialization fails.
 */
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
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win,
		ctrl->game->img, 0, 0);
}
