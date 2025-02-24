/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:19:57 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 10:18:42 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Handles the event when the spacebar key is pressed.
// If the spacebar (SPACE) is detected, sets the shooting flag to 1
// and calls the shoot() function to process the shooting logic.
// Returns 0 on success, or 1 if the game controller is NULL.

int	space_press(int keycode, t_ctrl *ctrl)
{
	if (!ctrl->game)
		return (1);
	if (keycode == SPACE)
	{
		ctrl->game->is_shooting = 1;
		shoot(ctrl);
	}
	return (0);
}

// Handles the event when a mouse click is detected.
// If the primary mouse button (MOUSE_CLICK) is clicked,
// sets the shooting flag
// and calls the shoot() function to execute the shooting action.
// The x and y parameters are unused and explicitly cast to void.

int	mouse_click(int button, int x, int y, t_ctrl *ctrl)
{
	(void)x;
	(void)y;
	if (!ctrl->game)
		return (1);
	if (button == MOUSE_CLICK)
	{
		ctrl->game->is_shooting = 1;
		shoot(ctrl);
	}
	return (0);
}

// Initializes all the necessary event hooks for the game window.
// - Hides the mouse cursor and centers it on the window.
// - Sets up key press/release, mouse movement, and click hooks.
// - Links the draw_loop function to the main rendering loop.
// - Ensures that the window closes properly on exit signals.

void	init_hooks(t_ctrl *ctrl)
{
	mlx_mouse_hide(ctrl->game->mlx, ctrl->game->win);
	mlx_mouse_move(ctrl->game->mlx, ctrl->game->win, WIDTH / 2, HEIGHT / 2);
	mlx_loop_hook(ctrl->game->mlx, draw_loop, ctrl);
	mlx_hook(ctrl->game->win, 6, 1L << 6, mouse_move, ctrl);
	mlx_hook(ctrl->game->win, 2, 1L << 0, key_press, ctrl);
	mlx_hook(ctrl->game->win, 3, 1L << 1, key_release, ctrl);
	mlx_mouse_hook(ctrl->game->win, mouse_click, ctrl);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
}

// Initializes the main game window and rendering image buffer.
// - Sets up the MLX graphical context.
// - Creates the main window with specified WIDTH and HEIGHT.
// - Initializes the image buffer for rendering frames.
// - Sets pointers to image data for direct pixel manipulation.
// - Handles cleanup and exits if initialization fails at any step.

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
	ctrl->game->fight.alive_enemies = ctrl->map.enemies_counter;
	ctrl->game->debug = 0;
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
}
