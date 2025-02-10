/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:13:30 by stefan            #+#    #+#             */
/*   Updated: 2025/02/10 20:40:37 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

static int	load_frame(t_game *game, t_texture *texture, char *path, int index)
{
	int	width;
	int	height;

	width = TEX_WIDTH;
	height = TEX_HEIGHT;
	texture->frames[index] = mlx_xpm_file_to_image(game->mlx, path, &width,
			&height);
	if (!texture->frames[index])
	{
		if (path == NULL)
			printf("ERROR: path is NULL\n");
		else
			printf("ERROR: mlx_xpm_file_to_image failed for path: %s\n", path);
		return (0);
	}
	texture->frames_addr[index] = mlx_get_data_addr(texture->frames[index],
			&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (!texture->frames_addr[index])
	{
		printf("ERROR: Failed to get texture data at %s\n", path);
		// mlx_destroy_image(game->mlx, texture->frames[index]);
		// texture->frames[index] = NULL;
		return (0);
	}
	return (1);
}

int	load_texture(t_game *game, t_texture *texture, char **frames)
{
	int	i;
	int	loaded;

	loaded = 0;
	i = 0;
	while (frames[i])
	{
		if (!load_frame(game, texture, frames[i], i))
		{
			//free_textures(game, texture);
			//close_window(ctrl);
			return (0);
		}
		printf("TEXTURES: %s\n", frames[i]);
		loaded++;
		i++;
	}
	texture->frames_count = loaded;
	texture->current_frame = 0;
	return (1);
}
