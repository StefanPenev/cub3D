/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/24 21:33:08 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_map(char **map, size_t rows)
{
	size_t	i;

	i = 0;
	if (!map)
		return ;
	while (i < rows)
	{
		if (map[i])
			free(map[i]);
		i++;
	}
	free(map);
}

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
	{
		free_map(ctrl->map.full_map, ctrl->map.rows);
	}
	if (ctrl->game->north_texture.path)
		free(ctrl->game->north_texture.path);
	if (ctrl->game->south_texture.path)
		free(ctrl->game->south_texture.path);
	if (ctrl->game->west_texture.path)
		free(ctrl->game->west_texture.path);
	if (ctrl->game->east_texture.path)
		free(ctrl->game->east_texture.path);
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
	game_cleanup(ctrl);
	exit(1);
}

void	cleanup_textures(t_game *game)
{
	if (game->north_texture.img)
		mlx_destroy_image(game->mlx, game->north_texture.img);
	if (game->south_texture.img)
		mlx_destroy_image(game->mlx, game->south_texture.img);
	if (game->east_texture.img)
		mlx_destroy_image(game->mlx, game->east_texture.img);
	if (game->west_texture.img)
		mlx_destroy_image(game->mlx, game->west_texture.img);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
}

int	close_window(t_ctrl *ctrl)
{
	if (!ctrl)
		exit(EXIT_FAILURE);
	cleanup_textures(ctrl->game);
	if (ctrl->game->mlx && ctrl->game->win)
		mlx_destroy_window(ctrl->game->mlx, ctrl->game->win);
	if (ctrl->game->mlx)
	{
		mlx_destroy_display(ctrl->game->mlx);
		mlx_loop_end(ctrl->game->mlx);
		free(ctrl->game->mlx);
	}
	game_cleanup(ctrl);
	exit(EXIT_SUCCESS);
}
