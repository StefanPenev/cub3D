/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/22 21:54:11 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	load_texture(t_game *game, t_texture *texture, char *path)
{
	texture->img = mlx_xpm_file_to_image(game->mlx, path, &(int){TEX_WIDTH},
			&(int){TEX_HEIGHT});
	if (!texture->img)
	{
		printf("Error: Failed to load texture at %s\n", path);
		exit(EXIT_FAILURE);
	}
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		printf("Error: Failed to get texture data at %s\n", path);
		exit(EXIT_FAILURE);
	}
}

void	load_all_textures(t_game *game)
{
	load_texture(game, &game->north_texture, game->north_texture.path);
	load_texture(game, &game->south_texture, game->south_texture.path);
	load_texture(game, &game->east_texture, game->east_texture.path);
	load_texture(game, &game->west_texture, game->west_texture.path);
}

int	main(int argc, char **argv)
{
	t_ctrl	*ctrl;

	ctrl = malloc(sizeof(t_ctrl));
	if (!ctrl)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	if (init_ctrl(ctrl) != 0)
		return (1);
	check_args(argc, argv, ctrl);
	parse_map(argv[1], ctrl);
	// DEBUG
	for (int i = 0; ctrl->map.full_map[i] != NULL; i++)
	{
		printf("DEBUG: %s\n", ctrl->map.full_map[i]);
	}
	printf("SUCCESS\n");
	// DEBUG
	init_game_window(ctrl->game);
	load_all_textures(ctrl->game);
	init_player(&ctrl->game->player, ctrl->map.player_position.x,
		ctrl->map.player_position.y, ctrl->map.player_position.orientation);
	mlx_loop_hook(ctrl->game->mlx, draw_loop, ctrl);
	mlx_hook(ctrl->game->win, 2, 1L << 0, key_press, ctrl);
	mlx_hook(ctrl->game->win, 3, 1L << 1, key_release, ctrl);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
	mlx_loop(ctrl->game->mlx);
	return (0);
}
