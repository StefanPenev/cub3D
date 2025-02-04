/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:23:10 by stefan            #+#    #+#             */
/*   Updated: 2025/02/04 14:46:24 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	init_player(t_player *player, float start_x, float start_y,
		float orientation)
{
	player->x = start_x;
	player->y = start_y;
	player->angle = orientation;
	player->key_up = false;
	player->key_down = false;
	player->key_right = false;
	player->key_left = false;
	player->left_rotate = false;
	player->right_rotate = false;
}

void	init_texture(t_texture *texture)
{
	int	i;

	i = 0;
	ft_memset(texture, 0, sizeof(t_texture));
	texture->path = NULL;
	texture->frames = malloc(MAX_FRAMES * sizeof(void *));
	texture->paths = malloc(MAX_FRAMES * sizeof(void *));
	if (!texture->frames || !texture->paths)
	{
		printf("Error: Failed to allocate memory for texture frames\n");
		exit(EXIT_FAILURE);
	}
	while (i < MAX_FRAMES)
	{
		texture->frames[i] = NULL;
		texture->paths[i] = NULL;
		i++;
	}
}

void	init_game(t_game *game)
{
	ft_memset(game, 0, sizeof(t_game));
	init_texture(&game->north_texture);
	init_texture(&game->south_texture);
	init_texture(&game->east_texture);
	init_texture(&game->west_texture);
	game->floor_color = 0xFFFFFFFF;
	game->ceiling_color = 0xFFFFFFFF;
	game->is_shooting = 0;
	game->shoot_ac = 0;
}

void	init_map(t_map *map)
{
	map->full_map = NULL;
	map->map_tmp = NULL;
	map->player_position.x = 0;
	map->player_position.y = 0;
	map->player_index.x = 0;
	map->player_index.y = 0;
	map->rows = 0;
	map->columns = 0;
	map->players_count = 0;
	map->has_free_way = 0;
	map->textures_defined = 0;
	map->colors_defined = 0;
	map->map_started = 0;
	map->doors_counter = 0; //bonus
	map->doors = NULL; //bonus
}

int	init_ctrl(t_ctrl *ctrl)
{
	init_map(&ctrl->map);
	ctrl->anim.fc = 0;
	ctrl->anim.ac = 0;
	ctrl->gnl.save = NULL;
	ctrl->gnl.line = NULL;
	ctrl->game = malloc(sizeof(t_game));
	if (!ctrl->game)
		return (1);
	init_game(ctrl->game);
	return (0);
}
