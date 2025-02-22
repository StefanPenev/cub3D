/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_controls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:54:17 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 13:03:02 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	draw_texture_pixels(t_game *game, int x, int y, t_texture *texture)
{
	int		i;
	int		j;
	size_t	color;

	i = 0;
	while (i < texture->height)
	{
		j = 0;
		while (j < texture->width)
		{
			color = get_texture_color(texture, j, i);
			if ((color & 0xFFFFFF) != 0x000000 && (color & 0xFFFFFF) > 0x010101)
			{
				if ((x + j) >= 0 && (x + j) < WIDTH && (y + i) >= 0
					&& (y + i) < HEIGHT)
				{
					put_pixel(x + j, y + i, color, game);
				}
			}
			j++;
		}
		i++;
	}
}

void	draw_controls_button(t_game *game)
{
	int	x;
	int	y;

	if (!game)
	{
		printf("ERROR: Controls texture not loaded!\n");
		return ;
	}
	x = WIDTH - game->controls_button.width - 10;
	y = 10;
	draw_texture_pixels(game, x, y, &game->controls_button);
}

void	draw_controls(t_game *game)
{
	int	x;
	int	y;

	if (!game)
	{
		printf("ERROR: Controls texture not loaded!\n");
		return ;
	}
	x = (WIDTH / 2) - (game->controls.width / 2);
	y = (HEIGHT / 2) - (game->controls.height / 2);
	draw_texture_pixels(game, x, y, &game->controls);
}
