/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/10 16:32:21 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

// static void	load_door_texture(t_game *game, t_texture *texture, char *path,
// 		t_ctrl *ctrl)
// {
// 	texture->img = mlx_xpm_file_to_image(game->mlx, path, &(int){TEX_WIDTH},
// 			&(int){TEX_HEIGHT});
// 	if (!texture->img)
// 	{
// 		printf("Error: Failed to load texture at %s\n", path);
// 		close_window(ctrl);
// 	}
// 	else
// 	{
// 		printf("DEBUG: Door texture loaded successfully\n");
// 	}
// 	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
// 			&texture->line_length, &texture->endian);
// 	if (!texture->addr)
// 	{
// 		printf("Error: Failed to get texture data at %s\n", path);
// 		close_window(ctrl);
// 	}
// }

// void	weapon_textures(t_ctrl *ctrl)
// {
// 	int	weapon_width;
// 	int	weapon_height;

// 	weapon_width = TEX_WIDTH;
// 	weapon_height = TEX_HEIGHT;
// 	ctrl->game->weapon_idle.path = "./textures/gun_idle.xpm";
// 	ctrl->game->weapon_shoot.path = "./textures/gun_shoot.xpm";
// 	load_tex(weapon_width, weapon_height, &ctrl->game->weapon_idle, ctrl);
// 	load_tex(weapon_width, weapon_height, &ctrl->game->weapon_shoot, ctrl);
// }

void	free_loaded_frames(int i, t_texture *tex, t_ctrl *ctrl)
{
	printf("Error: Failed to load texture at %s\n", tex->paths[i]);
	while (--i >= 0)
	{
		if (tex->paths[i])
			mlx_destroy_image(ctrl->game->mlx, tex->paths[i]);
		tex->paths[i] = NULL;
	}
	free(tex->paths);
	tex->paths = NULL;
	close_window(ctrl);
}

static void	load_single_texture(int width, int height, t_texture *tex,
		t_ctrl *ctrl)
{
	tex->img = mlx_xpm_file_to_image(ctrl->game->mlx, tex->path, &width,
			&height);
	if (!tex->img)
	{
		printf("ERROR: Failed to load texture at: %s\n", tex->path);
		close_window(ctrl);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
	if (!tex->addr)
	{
		printf("Error: Failed to get texture data at %s\n", tex->path);
		close_window(ctrl);
	}
}

void	load_framed_texture(int width, int height, t_texture *tex, t_ctrl *ctrl)
{
	int	i;

	i = 0;
	while (i < MAX_FRAMES)
	{
		if (!tex->paths[i])
		{
			printf("Error: Texture frame %d is NULL\n", i);
			i++;
			continue ;
		}
		tex->frames[i] = mlx_xpm_file_to_image(ctrl->game->mlx, tex->paths[i],
				&width, &height);
		if (!tex->frames[i])
			free_loaded_frames(i, tex, ctrl);
		tex->frames_addr[i] = mlx_get_data_addr(tex->frames[i],
				&tex->bits_per_pixel, &tex->line_length, &tex->endian);
		if (!tex->frames_addr[i])
		{
			printf("Error: Failed to get texture data at %s\n", tex->paths[i]);
			close_window(ctrl);
		}
		i++;
	}
}

void	load_enemy_textures(t_game *game)
{
	int	enemy_width;
	int	enemy_height;

	enemy_width = TEX_WIDTH;
	enemy_height = TEX_HEIGHT;
	game->enemy.path = "./textures/enemy/e_0.xpm";
	// use load_single_texture
	game->enemy.img = mlx_xpm_file_to_image(game->mlx, game->enemy.path,
			&enemy_width, &enemy_height);
	if (!game->enemy.img)
	{
		printf("ERROR: Failed to load enemy texture: %s\n", game->enemy.path);
		exit(1);
	}
	game->enemy.addr = mlx_get_data_addr(game->enemy.img,
			&game->enemy.bits_per_pixel, &game->enemy.line_length,
			&game->enemy.endian);
}

void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	int	width;
	int	height;

	width = TEX_WIDTH;
	height = TEX_HEIGHT;
	game->door.path = "./textures/door.xpm";
	game->weapon_idle.path = "./textures/gun_idle.xpm";
	game->weapon_shoot.path = "./textures/gun_shoot.xpm";
	load_framed_texture(width, height, &game->north_texture, ctrl);
	load_framed_texture(width, height, &game->south_texture, ctrl);
	load_framed_texture(width, height, &game->east_texture, ctrl);
	load_framed_texture(width, height, &game->west_texture, ctrl);
	load_single_texture(width, height, &game->weapon_idle, ctrl);
	load_single_texture(width, height, &game->weapon_shoot, ctrl);
	load_single_texture(width, height, &game->door, ctrl);
	load_enemy_textures(game);
}
