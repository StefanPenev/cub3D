/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_all_textures.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:42:56 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/21 10:40:13 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

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
