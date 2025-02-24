/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/24 14:14:24 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Cleans up all dynamically allocated memory related to the game controller.
// Frees the map, doors, enemies, trigonometric tables,
// the game structure itself, and finally the controller.
// Sets pointers to NULL after freeing to avoid dangling references.

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
		free_map(ctrl->map.full_map, ctrl->map.rows);
	if (ctrl->map.doors)
	{
		free(ctrl->map.doors);
		ctrl->map.doors = NULL;
	}
	if (ctrl->map.enemies)
	{
		free(ctrl->map.enemies);
		ctrl->map.enemies = NULL;
	}
	if (ctrl->trig_tables)
	{
		free_trig_tables(ctrl->trig_tables);
	}
	free(ctrl->game);
	ctrl->game = NULL;
	free(ctrl);
	ctrl = NULL;
}

// Handles a clean exit from the game with an optional message.
// If a message is provided,
// it is printed before closing the window and exiting.

void	clean_exit(char *str, t_ctrl *ctrl)
{
	if (str != NULL)
	{
		printf("%s", str);
	}
	close_window(ctrl, 1);
}

// Frees all textures associated with the game.
// Iterates through each framed texture (including player, enemy, UI textures)
// and frees them.
// Also destroys the main image buffer if it exists.

void	cleanup_textures(t_game *game)
{
	free_framed_texture(game, &game->north_texture);
	free_framed_texture(game, &game->south_texture);
	free_framed_texture(game, &game->west_texture);
	free_framed_texture(game, &game->east_texture);
	free_framed_texture(game, &game->enemy);
	free_framed_texture(game, &game->crosshair);
	free_framed_texture(game, &game->ceiling_texture);
	free_framed_texture(game, &game->floor_texture);
	free_framed_texture(game, &game->weapon_idle);
	free_framed_texture(game, &game->weapon_shoot);
	free_framed_texture(game, &game->door);
	free_framed_texture(game, &game->lose_img);
	free_framed_texture(game, &game->win_img);
	free_framed_texture(game, &game->controls);
	free_framed_texture(game, &game->controls_button);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
}

// Completely shuts down the game and releases all allocated resources.
// - Cleans up all textures and memory allocations.
// - Ends the MLX loop and destroys the window.
// - Cleans up the MLX display and exits the program successfully.

int	close_window(t_ctrl *ctrl, int error_flag)
{
	if (!ctrl)
		exit(EXIT_FAILURE);
	cleanup_textures(ctrl->game);
	if (ctrl->game->mlx)
		mlx_loop_end(ctrl->game->mlx);
	if (ctrl->game->mlx && ctrl->game->win)
	{
		mlx_destroy_window(ctrl->game->mlx, ctrl->game->win);
		ctrl->game->win = NULL;
	}
	if (ctrl->game->mlx)
	{
		mlx_destroy_display(ctrl->game->mlx);
		free(ctrl->game->mlx);
		ctrl->game->mlx = NULL;
	}
	game_cleanup(ctrl);
	if (error_flag)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}
