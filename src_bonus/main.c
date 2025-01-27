/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/27 14:41:30 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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
	init_game_window(ctrl);
	load_all_textures(ctrl->game, ctrl);
	init_player(&ctrl->game->player, ctrl->map.player_position.x,
		ctrl->map.player_position.y, ctrl->map.player_position.orientation);
	init_hooks(ctrl);
	mlx_loop(ctrl->game->mlx);
	game_cleanup(ctrl);
	return (0);
}
