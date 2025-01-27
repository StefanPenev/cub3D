/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:50 by stefan            #+#    #+#             */
/*   Updated: 2025/01/27 14:40:39 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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

void	load_all_textures(t_game *game, t_ctrl *ctrl)
{
	load_texture(game, &game->north_texture, game->north_texture.path, ctrl);
	load_texture(game, &game->south_texture, game->south_texture.path, ctrl);
	load_texture(game, &game->east_texture, game->east_texture.path, ctrl);
	load_texture(game, &game->west_texture, game->west_texture.path, ctrl);
}
