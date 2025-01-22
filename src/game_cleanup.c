/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:40:22 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/22 14:22:37 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_map(char **map, size_t rows)
{
	size_t	i;

	i = 0;
	if (!map)
		return ;
	while (i < rows)
	{
		if (map[i])
			free(map[i]);
		i++;
	}
	free(map);
}

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
	{
		free_map(ctrl->map.full_map, ctrl->map.rows);
	}
	if (ctrl->game->north_texture.path)
	{
		free(ctrl->game->north_texture.path);
		// printf("DEBUG: North texture path WAS FREED\n");
	}
	if (ctrl->game->south_texture.path)
	{
		free(ctrl->game->south_texture.path);
		// printf("DEBUG: South texture path WAS FREED\n");
	}
	if (ctrl->game->west_texture.path)
	{
		free(ctrl->game->west_texture.path);
		// printf("DEBUG: West texture path WAS FREED\n");
	}
	if (ctrl->game->east_texture.path)
	{
		free(ctrl->game->east_texture.path);
		// printf("DEBUG: East texture path WAS FREED\n");
	}
	free(ctrl->game);
	ctrl->game = NULL;
	free(ctrl);
	ctrl = NULL;
}

void	clean_exit(char *str, t_ctrl *ctrl)
{
	if (str != NULL)
	{
		printf("%s", str);
	}
	game_cleanup(ctrl);
	exit(1);
}
