/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:50:43 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 16:55:45 by anilchen         ###   ########.fr       */
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

// void	free_single_texture(t_game *game)
// {
// 	if (game->door.img)
// 	{
// 		mlx_destroy_image(game->mlx, game->door.img);
// 		game->door.img = NULL;
// 	}
// 	if (game->weapon_idle.img)
// 	{
// 		mlx_destroy_image(game->mlx, game->weapon_idle.img);
// 		game->weapon_idle.img = NULL;
// 	}
// 	if (game->weapon_shoot.img)
// 	{
// 		mlx_destroy_image(game->mlx, game->weapon_shoot.img);
// 		game->weapon_shoot.img = NULL;
// 	}
// }

void	free_framed_texture(t_game *game, t_texture *texture)
{
	int	i;

	if (!texture)
		return ;
	if (texture->frames)
	{
		i = 0;
		while (texture->frames[i] != NULL)
		{
			if (texture->frames[i])
			{
				mlx_destroy_image(game->mlx, texture->frames[i]);
				texture->frames[i] = NULL;
			}
			if (texture->paths && texture->paths[i])
			{
				free(texture->paths[i]);
				texture->paths[i] = NULL;
			}
			i++;
		}
		free(texture->frames);
		texture->frames = NULL;
	}
	if (texture->paths)
	{
		free(texture->paths);
		texture->paths = NULL;
	}
	if (texture->frames_addr)
	{
		free(texture->frames_addr);
		texture->frames_addr = NULL;
	}
}
