/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 16:10:24 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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

void	clean_exit(char *str, t_ctrl *ctrl)
{
	if (str != NULL)
	{
		printf("%s", str);
	}
	close_window(ctrl);
}

void	cleanup_textures(t_game *game)
{
	free_framed_texture(game, &game->north_texture);
	free_framed_texture(game, &game->south_texture);
	free_framed_texture(game, &game->west_texture);
	free_framed_texture(game, &game->east_texture);
	free_framed_texture(game, &game->enemy);
	free_framed_texture(game, &game->crosshair);
	free_framed_texture(game, &game->ceiling_texture);
	free_framed_texture(game, &game->weapon_idle);
	free_framed_texture(game, &game->weapon_shoot);
	free_framed_texture(game, &game->door);
	free_framed_texture(game, &game->lose_img);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
}

int	close_window(t_ctrl *ctrl)
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
	exit(EXIT_SUCCESS);
}
