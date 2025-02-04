/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/04 15:00:00 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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

void free_weapon_textures(t_game *game)
{
    if (game->weapon_idle.img)
    {
        mlx_destroy_image(game->mlx, game->weapon_idle.img);
        game->weapon_idle.img = NULL;
    }
    if (game->weapon_shoot.img)
    {
        mlx_destroy_image(game->mlx, game->weapon_shoot.img);
        game->weapon_shoot.img = NULL;
    }
}

void	free_texture(t_game *game, t_texture *texture)
{
	int	i;

	i = 0;
	if (texture && texture->frames)
	{
		while (i < MAX_FRAMES)
		{
			if (texture->frames[i])
			{
				mlx_destroy_image(game->mlx, texture->frames[i]);
				free(texture->paths[i]);
				texture->frames[i] = NULL;
				texture->paths[i] = NULL;
			}
			i++;
		}
		free(texture->frames);
		texture->frames = NULL;
		free(texture->paths);
		texture->paths = NULL;
	}
	if (texture->path)
	{
		mlx_destroy_image(game->mlx, texture->img);
		free(texture->path);
		texture->img = NULL;
		texture->path = NULL;
	}
}

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
		free_map(ctrl->map.full_map, ctrl->map.rows);
	// free_texture(ctrl->game, &ctrl->game->north_texture);
	// free_texture(ctrl->game, &ctrl->game->south_texture);
	// free_texture(ctrl->game, &ctrl->game->west_texture);
	// free_texture(ctrl->game, &ctrl->game->east_texture);
	if (ctrl->map.doors)
	{
		free(ctrl->map.doors);
		ctrl->map.doors = NULL;
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
	free_texture(game, &game->north_texture);
	printf("DEBUG: north_texture freed\n");
	free_texture(game, &game->south_texture);
	printf("DEBUG: south_texture freed\n");
	free_texture(game, &game->west_texture);
	printf("DEBUG: west_texture freed\n");
	free_texture(game, &game->east_texture);
	printf("DEBUG: east_texture freed\n");
	free_texture(game, &game->door);
	printf("DEBUG: door freed\n");
	free_weapon_textures(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
}
// void	cleanup_textures(t_game *game)
// {
// 	if (game->north_texture.img)
// 		mlx_destroy_image(game->mlx, game->north_texture.img);
// 	if (game->south_texture.img)
// 		mlx_destroy_image(game->mlx, game->south_texture.img);
// 	if (game->east_texture.img)
// 		mlx_destroy_image(game->mlx, game->east_texture.img);
// 	if (game->west_texture.img)
// 		mlx_destroy_image(game->mlx, game->west_texture.img);
// 	if (game->img)
// 		mlx_destroy_image(game->mlx, game->img);
// }

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
