/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:13:30 by stefan            #+#    #+#             */
/*   Updated: 2025/02/20 13:28:03 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

// Loads a single frame of a texture from an XPM
// file using the MiniLibX library.
// - game: Pointer to the t_game structure containing the MLX instance.
// - texture: Pointer to the texture structure where the frame will be stored.
// - path: File path to the XPM texture file.
// - index: Index in the texture's frame array where the
// loaded image will be stored.
// Returns 1 if the frame is successfully loaded, otherwise returns 0.

static int	load_frame(t_game *game, t_texture *texture, char *path, int index)
{
	texture->frames[index] = mlx_xpm_file_to_image(game->mlx, path,
			&texture->width, &texture->height);
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
		return (0);
	}
	return (1);
}

// Loads all frames for a given texture from an array of file paths.
// - game: Pointer to the t_game structure containing the MLX instance.
// - texture: Pointer to the texture structure where frames will be stored.
// - frames: Array of file paths to XPM texture files.
// Returns 1 if all frames are successfully loaded, otherwise returns 0.

int	load_texture(t_game *game, t_texture *texture, char **frames)
{
	int	i;
	int	loaded;

	loaded = 0;
	i = 0;
	while (frames[i])
	{
		if (!load_frame(game, texture, frames[i], i))
			return (0);
		loaded++;
		i++;
	}
	texture->frames_count = loaded;
	texture->current_frame = 0;
	return (1);
}
