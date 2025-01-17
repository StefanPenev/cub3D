/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/17 09:53:37 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

//Only for test purposes
void	draw_red_square(t_game *game, int x, int y, int size)
{
	int	i;
	int	j;
	int	color;
	int	pixel_x;
	int	pixel_y;
	int	pixel_index;

	color = 0xFF0000;
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			pixel_x = x + i;
			pixel_y = y + j;
			if (pixel_x >= 0 && pixel_y >= 0)
			{
				pixel_index = pixel_y * game->size_line
					+ (pixel_x * (game->bpp / 8));
				*(int *)(game->img_data + pixel_index) = color;
			}
			j++;
		}
		i++;
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
	draw_red_square(game, 100, 100, 50);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int	close_window(void)
{
	// mlx_destroy_image(game->mlx, game->img);
	// mlx_destroy_window(game->mlx, game->win);
	// mlx_destroy_display(game->mlx);
	//free(game->mlx);
	exit(0);
}

int	main(int argc, char **argv)
{
	t_ctrl	*ctrl;
	//t_game	*game;

	ctrl = malloc(sizeof(t_ctrl));
	if (!ctrl)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	ctrl->map.full_map = NULL;
	ctrl->map.players_positions = NULL;
	ctrl->map.rows = 0;
	ctrl->map.columns = 0;
	ctrl->map.players_count = 0;
	ctrl->map.has_free_way = 0;
	check_args(argc, argv, ctrl);
	parse_map(argv[1], ctrl);
	// DEBUG
	for (int i = 0; ctrl->map.full_map[i] != NULL; i++)
	{
		printf("DEBUG: %s\n", ctrl->map.full_map[i]);
	}
	printf("SUCCESS\n");
	// DEBUG
	ctrl->game = malloc(sizeof(t_game));
	if (!ctrl->game)
		return (EXIT_FAILURE);
	init_game_window(ctrl->game);
	mlx_hook(ctrl->game->win, 2, 1L << 0, hook_keypress, ctrl);
	mlx_hook(ctrl->game->win, 17, 0, close_window, ctrl);
	mlx_loop(ctrl->game->mlx);
	game_cleanup(ctrl);
	return (0);
}
