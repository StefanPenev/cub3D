/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/10 20:54:17 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	load_weapon_textures(t_game *game)
{
	char	*weapon_idle_frames[2];
	char	*weapon_shoot_frames[2];

	init_texture(&game->weapon_idle, 2);
	weapon_idle_frames[0] = "./textures/gun_idle.xpm";
	weapon_idle_frames[1] = NULL;
	if (!load_texture(game, &game->weapon_idle, weapon_idle_frames))
	{
		printf("ERROR: Failed to load weapon_idle texture.\n");
		return ;
	}
	init_texture(&game->weapon_shoot, 2);
	weapon_shoot_frames[0] = "./textures/gun_shoot.xpm";
	weapon_shoot_frames[1] = NULL;
	if (!load_texture(game, &game->weapon_shoot, weapon_shoot_frames))
	{
		printf("ERROR: Failed to load weapon_shoot texture.\n");
		return ;
	}
}

void	load_wall_textures(t_ctrl *ctrl)
{
	load_texture(ctrl->game, &ctrl->game->north_texture,
		ctrl->game->north_texture.paths);
	load_texture(ctrl->game, &ctrl->game->south_texture,
		ctrl->game->south_texture.paths);
	load_texture(ctrl->game, &ctrl->game->east_texture,
		ctrl->game->east_texture.paths);
	load_texture(ctrl->game, &ctrl->game->west_texture,
		ctrl->game->west_texture.paths);
}

// void	load_enemy_textures(t_game *game)
// {
// 	char	*enemy_frames[2];

// 	init_texture(&game->enemy, 2);
// 	game->enemy.path = "./textures/enemy/e_0.xpm";
// 	enemy_frames[0] = game->enemy.path;
// 	enemy_frames[1] = NULL;
// 	load_texture(game, &game->enemy, enemy_frames);
// }
void load_enemy_textures(t_game *game)
{
    char *enemy_frames[] = {
        "./textures/enemy/e_0.xpm",
        "./textures/enemy/e_1.xpm",
        "./textures/enemy/e_2.xpm",
        "./textures/enemy/e_3.xpm",
        "./textures/enemy/e_4.xpm",
        NULL
    };

    init_texture(&game->enemy, 5);
    load_texture(game, &game->enemy, enemy_frames);
}

void	load_door_textures(t_game *game)
{
	char	*door_frames[2];

	init_texture(&game->door, 2);
	game->door.path = "./textures/door.xpm";
	door_frames[0] = game->door.path;
	door_frames[1] = NULL;
	load_texture(game, &game->door, door_frames);
}

void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	load_wall_textures(ctrl);
	load_weapon_textures(game);
	load_door_textures(game);
	load_enemy_textures(game);
}
