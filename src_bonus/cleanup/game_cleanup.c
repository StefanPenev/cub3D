/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/10 16:54:51 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// void	free_map(char **map, size_t rows)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!map)
// 		return ;
// 	while (i < rows)
// 	{
// 		if (map[i])
// 			free(map[i]);
// 		i++;
// 	}
// 	free(map);
// }

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
// 	if (game->enemy.img)
// 	{
// 		mlx_destroy_image(game->mlx, game->enemy.img);
// 		game->enemy.img = NULL;
// 	}
// }

// void	free_framed_texture(t_game *game, t_texture *texture)
// {
// 	int	i;

// 	i = 0;
// 	if (texture && texture->frames)
// 	{
// 		while (i < MAX_FRAMES)
// 		{
// 			if (texture->frames[i])
// 			{
// 				mlx_destroy_image(game->mlx, texture->frames[i]);
// 				free(texture->paths[i]);
// 				texture->frames[i] = NULL;
// 				texture->paths[i] = NULL;
// 			}
// 			i++;
// 		}
// 		free(texture->frames);
// 		texture->frames = NULL;
// 		free(texture->paths);
// 		texture->paths = NULL;
// 		free(texture->frames_addr);
// 		texture->frames_addr = NULL;
// 	}
// }

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
		free_map(ctrl->map.full_map, ctrl->map.rows);
	if (ctrl->map.doors)
	{
		free(ctrl->map.doors);
		ctrl->map.doors = NULL;
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
	//printf("DEBUG: north_texture freed\n");
	free_framed_texture(game, &game->south_texture);
	//printf("DEBUG: south_texture freed\n");
	free_framed_texture(game, &game->west_texture);
	//printf("DEBUG: west_texture freed\n");
	free_framed_texture(game, &game->east_texture);
	//printf("DEBUG: east_texture freed\n");
	free_single_texture(game);
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
