/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_UI_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:41:00 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/23 11:06:17 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Loads the texture for the player's crosshair.
// - Initializes the crosshair texture structure with width and height settings.
// - Loads the crosshair from an XPM file.

void	load_crosshair(t_game *game)
{
	char	*crosshair_frames[2];

	init_texture(&game->crosshair, 2);
	game->crosshair.paths[0] = ft_strdup("./textures/gun/crosshair.xpm");
	game->crosshair.paths[1] = NULL;
	game->crosshair.width = 40;
	game->crosshair.height = 24;
	crosshair_frames[0] = game->crosshair.paths[0];
	crosshair_frames[1] = NULL;
	load_texture(game, &game->crosshair, crosshair_frames);
}

// Loads the texture displaying the game control instructions.
// - Initializes the controls texture structure with specified width and height.
// - Loads the texture from an XPM file.

void	load_controls(t_game *game)
{
	char	*controls_frames[2];

	init_texture(&game->controls, 2);
	game->controls.paths[0] = ft_strdup("./textures/controls/controls.xpm");
	game->controls.paths[1] = NULL;
	game->controls.width = 300;
	game->controls.height = 300;
	controls_frames[0] = game->controls.paths[0];
	controls_frames[1] = NULL;
	load_texture(game, &game->controls, controls_frames);
}

// Loads the texture for the "Controls" button displayed in the UI.
// - Initializes the controls_button texture structure with width and height.
// - Loads the texture from an XPM file.

void	load_controls_button(t_game *game)
{
	char	*controls_b_frames[2];

	init_texture(&game->controls_button, 2);
	game->controls_button.paths[0] = ft_strdup("./textures/controls/controls3.xpm");
	game->controls_button.paths[1] = NULL;
	game->controls_button.width = 128;
	game->controls_button.height = 22;
	controls_b_frames[0] = game->controls_button.paths[0];
	controls_b_frames[1] = NULL;
	load_texture(game, &game->controls_button, controls_b_frames);
}

// Loads the "You Lose" texture displayed when the player loses the game.
// - Initializes the lose_img texture structure.
// - Attempts to load the texture and provides error feedback if loading fails.

void	load_lose_texture(t_game *game)
{
	char	*lose_frames[2];

	init_texture(&game->lose_img, 2);
	game->lose_img.path = "./textures/you_lose.xpm";
	lose_frames[0] = game->lose_img.path;
	lose_frames[1] = NULL;
	if (!load_texture(game, &game->lose_img, lose_frames))
	{
		printf("ERROR: Failed to load 'You Lose' texture: %s\n",
			game->lose_img.path);
		game->lose_img.frames[0] = NULL;
	}
}

// Loads the "You Win" texture displayed when the player wins the game.
// - Initializes the win_img texture structure.
// - Attempts to load the texture and provides error feedback if loading fails.

void	load_win_texture(t_game *game)
{
	char	*win_frames[2];

	init_texture(&game->win_img, 2);
	game->win_img.path = "./textures/win.xpm";
	win_frames[0] = game->win_img.path;
	win_frames[1] = NULL;
	if (!load_texture(game, &game->win_img, win_frames))
	{
		printf("ERROR: Failed to load 'You Lose' texture: %s\n",
			game->win_img.path);
		game->win_img.frames[0] = NULL;
	}
}
