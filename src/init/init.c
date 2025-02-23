/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:23:10 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:17:46 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_player - Initializes the player structure with starting position and
 * orientation.
 * @player: The player structure to initialize.
 * @start_x: The starting x-coordinate.
 * @start_y: The starting y-coordinate.
 * @orientation: The starting orientation angle.
 */
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

/**
 * init_texture - Initializes the texture structure to default values.
 * @texture: The texture structure to initialize.
 */
void	init_texture(t_texture *texture)
{
	ft_memset(texture, 0, sizeof(t_texture));
	texture->path = NULL;
}

/**
 * init_game - Initializes the game structure and its textures.
 * @game: The game structure to initialize.
 */
void	init_game(t_game *game)
{
	ft_memset(game, 0, sizeof(t_game));
	init_texture(&game->north_texture);
	init_texture(&game->south_texture);
	init_texture(&game->east_texture);
	init_texture(&game->west_texture);
	game->floor_color = 0xFFFFFFFF;
	game->ceiling_color = 0xFFFFFFFF;
}

/**
 * init_map - Initializes the map structure to default values.
 * @map: The map structure to initialize.
 */
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
}

/**
 * init_ctrl - Initializes the control structure and its game data.
 * @ctrl: The control structure to initialize.
 * 
 * Returns 0 on success, 1 on failure.
 */
int	init_ctrl(t_ctrl *ctrl)
{
	init_map(&ctrl->map);
	ctrl->gnl.save = NULL;
	ctrl->gnl.line = NULL;
	ctrl->game = malloc(sizeof(t_game));
	if (!ctrl->game)
		return (1);
	init_game(ctrl->game);
	return (0);
}
