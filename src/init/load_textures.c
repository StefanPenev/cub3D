/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:50 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:18:53 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * load_texture - Loads a texture image from a file path and retrieves its data.
 * @game: The game structure.
 * @texture: The texture structure to load.
 * @path: The file path of the texture.
 * @ctrl: The control structure, used for cleanup on error.
 */
static void	load_texture(t_game *game, t_texture *texture, char *path,
	t_ctrl *ctrl)
{
	texture->img = mlx_xpm_file_to_image(game->mlx, path, &(int){TEX_WIDTH},
			&(int){TEX_HEIGHT});
	if (!texture->img)
	{
		printf("Error: Failed to load texture at %s\n", path);
		close_window(ctrl);
		exit(EXIT_FAILURE);
	}
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		printf("Error: Failed to get texture data at %s\n", path);
		close_window(ctrl);
		exit(EXIT_FAILURE);
	}
}

/**
 * load_all_textures - Loads all the textures used in the game.
 * @game: The game structure.
 * @ctrl: The control structure.
 */
void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	load_texture(game, &game->north_texture, game->north_texture.path, ctrl);
	load_texture(game, &game->south_texture, game->south_texture.path, ctrl);
	load_texture(game, &game->east_texture, game->east_texture.path, ctrl);
	load_texture(game, &game->west_texture, game->west_texture.path, ctrl);
}
