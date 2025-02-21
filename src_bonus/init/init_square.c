/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_square.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:43:26 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 18:09:03 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Initializes and returns a t_square structure with the specified parameters.
// - x: The x-coordinate of the top-left corner of the square.
// - y: The y-coordinate of the top-left corner of the square.
// - size: The length of one side of the square (in pixels or units).
// - color: The color value of the square (could be RGB or another format).
// Returns the initialized t_square structure with the given attributes.

t_square	init_square(int x, int y, int size, int color)
{
	t_square	square;

	square.x = x;
	square.y = y;
	square.size = size;
	square.color = color;
	return (square);
}
