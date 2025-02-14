/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/14 16:54:18 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	load_weapon_textures(t_game *game)
{
	char	*weapon_idle_frames[2];
	char	*weapon_shoot_frames[2];

	init_texture(&game->weapon_idle, 2);
	game->weapon_idle.paths[0] = ft_strdup("./textures/gun_idle.xpm");
	game->weapon_idle.paths[1] = NULL;
	weapon_idle_frames[0] = game->weapon_idle.paths[0];
	weapon_idle_frames[1] = NULL;
	if (!load_texture(game, &game->weapon_idle, weapon_idle_frames))
	{
		printf("ERROR: Failed to load weapon_idle texture.\n");
		return ;
	}
	init_texture(&game->weapon_shoot, 2);
	game->weapon_shoot.paths[0] = ft_strdup("./textures/gun_shoot.xpm");
	game->weapon_shoot.paths[1] = NULL;
	weapon_shoot_frames[0] = game->weapon_shoot.paths[0];
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

void	load_enemy_textures(t_game *game)
{
	char	*enemy_frames[7];

	// char	*enemy_frames[8];
	init_texture(&game->enemy, 7);
	game->enemy.paths[0] = ft_strdup("./textures/enemy/e_0.xpm");
	game->enemy.paths[1] = ft_strdup("./textures/enemy/e_1.xpm");
	game->enemy.paths[2] = ft_strdup("./textures/enemy/e_2.xpm");
	game->enemy.paths[3] = ft_strdup("./textures/enemy/e_3.xpm");
	game->enemy.paths[4] = ft_strdup("./textures/enemy/e_4.xpm");
	game->enemy.paths[5] = ft_strdup("./textures/enemy/e_5.xpm");
	game->enemy.paths[6] = NULL;
	enemy_frames[0] = game->enemy.paths[0];
	enemy_frames[1] = game->enemy.paths[1];
	enemy_frames[2] = game->enemy.paths[2];
	enemy_frames[3] = game->enemy.paths[3];
	enemy_frames[4] = game->enemy.paths[4];
	enemy_frames[5] = game->enemy.paths[5];
	enemy_frames[6] = NULL;
	// enemy_frames[6] = "./textures/enemy/e_corpse.xpm";
	// enemy_frames[7] = NULL;
	load_texture(game, &game->enemy, enemy_frames);
}

// void	load_enemy_textures(t_game *game)
// {
// 	//char	*enemy_frames[8];
// 	char	*enemy_frames[7];
// 	init_texture(&game->enemy, 7);
// 	enemy_frames[0] = "./textures/enemy/e_0.xpm";
// 	enemy_frames[1] = "./textures/enemy/e_1.xpm";
// 	enemy_frames[2] = "./textures/enemy/e_2.xpm";
// 	enemy_frames[3] = "./textures/enemy/e_3.xpm";
// 	enemy_frames[4] = "./textures/enemy/e_4.xpm";
// 	enemy_frames[5] = "./textures/enemy/e_5.xpm";
// 	enemy_frames[6] = NULL;
// 	//enemy_frames[6] = "./textures/enemy/e_corpse.xpm";
// 	//enemy_frames[7] = NULL;
// 	load_texture(game, &game->enemy, enemy_frames);
// }

void	load_door_textures(t_game *game)
{
	char	*door_frames[2];

	init_texture(&game->door, 2);
	game->door.paths[0] = ft_strdup("./textures/door.xpm");
	game->door.paths[1] = NULL;
	door_frames[0] = game->door.paths[0];
	door_frames[1] = NULL;
	load_texture(game, &game->door, door_frames);
}

void	load_crosshair(t_game *game)
{
	char	*crosshair_frames[2];

	init_texture(&game->crosshair, 2);
	game->crosshair.paths[0] = ft_strdup("./textures/crosshair.xpm");
	game->crosshair.paths[1] = NULL;
	game->crosshair.width = 40;
	game->crosshair.height = 24;
	crosshair_frames[0] = game->crosshair.paths[0];
	crosshair_frames[1] = NULL;
	load_texture(game, &game->crosshair, crosshair_frames);
}

void	load_lose_texture(t_game *game)
{
	char	*lose_frames[2];

	init_texture(&game->lose_img, 2);
	game->lose_img.path = "./textures/you_lose.xpm";
	lose_frames[0] = game->lose_img.path;
	lose_frames[1] = NULL;
	if (!load_texture(game, &game->lose_img, lose_frames))
	{
		printf("ERROR: Failed to load 'You Lose' texture: %s\n",
			game->lose_img.path);
		game->lose_img.frames[0] = NULL;
	}
}

void	load_ceiling_texture(t_game *game)
{
	char	*ceiling_frames[2];

	init_texture(&game->ceiling_texture, 2);
	game->ceiling_texture.paths[0] = ft_strdup("./textures/sky.xpm");
	game->ceiling_texture.paths[1] = NULL;
	ceiling_frames[0] = game->ceiling_texture.paths[0];
	ceiling_frames[1] = NULL;
	if (!load_texture(game, &game->ceiling_texture, ceiling_frames))
	{
		printf("ERROR: Failed to load 'Ceiling' texture.\n");
		game->ceiling_texture.frames[0] = NULL;
	}
}

void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	load_wall_textures(ctrl);
	load_weapon_textures(game);
	load_door_textures(game);
	load_crosshair(game);
	load_enemy_textures(game);
	load_lose_texture(game);
	load_ceiling_texture(game);
}
