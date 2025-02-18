/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:23:10 by stefan            #+#    #+#             */
/*   Updated: 2025/02/19 00:08:42 by stefan           ###   ########.fr       */
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
	player->hp = PLAYER_HP;
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = -player->dir_y * 0.66;
	player->plane_y = player->dir_x * 0.66;
}

void	init_texture(t_texture *texture, int max_frames)
{
	int	i;

	i = 0;
	ft_memset(texture, 0, sizeof(t_texture));
	texture->path = NULL;
	texture->frames = malloc(max_frames * sizeof(void *));
	texture->paths = malloc(max_frames * sizeof(void *));
	texture->frames_addr = malloc(max_frames * sizeof(void *));
	if (!texture->frames || !texture->paths || !texture->frames_addr)
	{
		printf("Error: Failed to allocate memory for texture frames\n");
		exit(EXIT_FAILURE);
	}
	while (i < max_frames)
	{
		texture->frames[i] = NULL;
		texture->paths[i] = NULL;
		texture->frames_addr[i] = NULL;
		i++;
	}
}

void	init_game(t_game *game)
{
	ft_memset(game, 0, sizeof(t_game));
	init_texture(&game->north_texture, 4);
	init_texture(&game->south_texture, 4);
	init_texture(&game->east_texture, 4);
	init_texture(&game->west_texture, 4);
	game->floor_color = 0xFFFFFFFF;
	game->ceiling_color = 0xFFFFFFFF;
	game->is_shooting = 0;
	game->shoot_ac = 0;
	game->fight.fight_started = 0;
	game->fight.enemy_shoot = 0;
	game->fight.player_shoot = 0;
	game->fight.shoot_delay = 80;
	game->fight.lose_flag = 0;
	game->fight.win_flag = 0;
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
	map->doors_counter = 0;
	map->enemies_counter = 0;
	map->doors = NULL;
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
	ctrl->trig_tables = init_trig_tables();
	if (!ctrl->trig_tables)
	{
		printf("Error: Failed to initialize trigonometric tables.\n");
		free(ctrl->game);
		return (1);
	}
	return (0);
}
