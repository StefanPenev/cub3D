/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:23:10 by stefan            #+#    #+#             */
/*   Updated: 2025/01/22 21:07:30 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

void	init_game_window(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		printf("Error: MLX initialization failed.\n");
		exit(1);
	}
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
	{
		printf("Error: Window creation failed.\n");
		free(game->mlx);
		exit(1);
	}
	game->img = mlx_new_image(game->mlx, 800, 600);
	if (!game->img)
	{
		printf("Error: Image creation failed.\n");
		mlx_destroy_window(game->mlx, game->win);
		free(game->mlx);
		exit(1);
	}
	game->img_data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	if (!game->img_data)
	{
		printf("Error: Image data not initialized.\n");
		mlx_destroy_image(game->mlx, game->img);
		mlx_destroy_window(game->mlx, game->win);
		free(game->mlx);
		exit(1);
	}
	game->debug = 0;
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int	init_ctrl(t_ctrl *ctrl)
{
	ctrl->map.full_map = NULL;
	ctrl->map.player_position.x = 0;
	ctrl->map.player_position.y = 0;
	ctrl->map.player_index.x = 0;
	ctrl->map.player_index.y = 0;
	ctrl->map.rows = 0;
	ctrl->map.columns = 0;
	ctrl->map.players_count = 0;
	ctrl->map.has_free_way = 0;
	ctrl->map.textures_defined = 0;
	ctrl->map.colors_defined = 0;
	ctrl->map.map_started = 0;
	ctrl->game = malloc(sizeof(t_game));
	if (!ctrl->game)
		return (1);
	ctrl->game->north_texture.path = NULL;
	ctrl->game->south_texture.path = NULL;
	ctrl->game->east_texture.path = NULL;
	ctrl->game->west_texture.path = NULL;
	ctrl->game->floor_color = 0xFFFFFFFF;
	ctrl->game->ceiling_color = 0xFFFFFFFF;
	return (0);
}
