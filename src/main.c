/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 15:04:20 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_game_window(t_game *game)
{
	printf("insode init_game_window");
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		return ;
	}
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
	{
		free(game->mlx);
		return ;
	}
}

int	main(int argc, char **argv)
{
	t_ctrl	*ctrl;

	// t_game	*game;
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
	game_cleanup(ctrl);
	// game = malloc(sizeof(t_game));
	// if (!game)
	// 	return (EXIT_FAILURE);
	// game->mlx = NULL;
	// game->win = NULL;
	mlx_hook(ctrl->game.win, 2, 1L << 0, hook_keypress, ctrl);
	// init_game_window(game);
	// mlx_loop(game->mlx);
	return (0);
}
