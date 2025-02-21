/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_all_textures.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:42:56 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/21 18:09:14 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Loads the floor texture, typically representing the sky
// - Initializes the floor_texture structure.
// - Loads the texture from an XPM file and handles errors.
void	load_floor_texture(t_game *game)
{
	char	*floor_frames[2];

	init_texture(&game->floor_texture, 2);
	game->floor_texture.path = "./textures/floor_tex2.xpm";
	floor_frames[0] = game->floor_texture.path;
	floor_frames[1] = NULL;
	if (!load_texture(game, &game->floor_texture, floor_frames))
	{
		printf("ERROR: Failed to load 'Floor' texture.\n");
		game->floor_texture.frames[0] = NULL;
	}
}

// Loads all required textures for the game, including walls, weapons, enemies,
// doors, UI elements (crosshair and controls),
// and game state screens (win/lose).
// Calls individual texture loading functions sequentially.

void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	load_wall_textures(ctrl);
	load_weapon_textures(game);
	load_door_textures(game);
	load_crosshair(game);
	load_enemy_textures(game);
	load_lose_texture(game);
	load_win_texture(game);
	load_ceiling_texture(game);
	load_controls(game);
	load_controls_button(game);
	load_floor_texture(game);
}
