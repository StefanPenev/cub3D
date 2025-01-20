/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/20 09:49:13 by stefan           ###   ########.fr       */
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
	game->img_data = mlx_get_data_addr(game->img, &game->bpp,
			&game->size_line, &game->endian);
	if (!game->img_data)
	{
		printf("Error: Image data not initialized.\n");
		mlx_destroy_image(game->mlx, game->img);
		mlx_destroy_window(game->mlx, game->win);
		free(game->mlx);
		exit(1);
	}
	game->ceiling_color = CEILING_COLOR;
	game->floor_color = FLOOR_COLOR;

	load_texture(game, &game->north_texture, "textures/north_wall.xpm");
    load_texture(game, &game->south_texture, "textures/south_wall.xpm");
    load_texture(game, &game->east_texture,  "textures/east_wall.xpm");
    load_texture(game, &game->west_texture,  "textures/west_wall.xpm");

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void	cleanup_textures(t_game *game)
{
	if (game->north_texture.img)
		mlx_destroy_image(game->mlx, game->north_texture.img);
	if (game->south_texture.img)
		mlx_destroy_image(game->mlx, game->south_texture.img);
	if (game->east_texture.img)
		mlx_destroy_image(game->mlx, game->east_texture.img);
	if (game->west_texture.img)
		mlx_destroy_image(game->mlx, game->west_texture.img);
}

int	close_window(void)
{
	// mlx_destroy_image(game->mlx, game->img);
	// mlx_destroy_window(game->mlx, game->win);
	// mlx_destroy_display(game->mlx);
	//free(game->mlx);
	exit(0);
}

int	init_ctrl(t_ctrl *ctrl)
{
	ctrl->map.full_map = NULL;
	ctrl->map.players_positions = NULL;
	ctrl->map.rows = 0;
	ctrl->map.columns = 0;
	ctrl->map.players_count = 0;
	ctrl->map.has_free_way = 0;
	ctrl->game = malloc(sizeof(t_game));
	if (!ctrl->game)
		return (1);
	init_game_window(ctrl->game);
	return (0);
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
	init_ctrl(ctrl);
	check_args(argc, argv, ctrl);
	parse_map(argv[1], ctrl);
	// DEBUG
	for (int i = 0; ctrl->map.full_map[i] != NULL; i++)
	{
		printf("DEBUG: %s\n", ctrl->map.full_map[i]);
	}
	printf("SUCCESS\n");
	// DEBUG

	//float player_start_x = 224.0f;
    //float player_start_y = 480.0f;
	float player_start_x = 96.0f;
    float player_start_y = 160.0f;
    float player_start_angle = M_PI / 2;

    init_player(&ctrl->game->player, player_start_x, player_start_y, player_start_angle);

	mlx_loop_hook(ctrl->game->mlx, draw_loop, ctrl);
	//mlx_hook(ctrl->game->win, 2, 1L << 0, hook_keypress, ctrl);
	mlx_hook(ctrl->game->win, 2, 1L << 0, key_press, ctrl);
    mlx_hook(ctrl->game->win, 3, 1L << 1, key_release, ctrl);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
	mlx_loop(ctrl->game->mlx);
	game_cleanup(ctrl);
	return (0);
}
