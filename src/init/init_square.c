/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_square.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:29:21 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 20:16:13 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_square - Initializes a square structure.
 * @x: The x-coordinate of the square's position.
 * @y: The y-coordinate of the square's position.
 * @size: The size of the square.
 * @color: The color of the square.
 * 
 * Returns a t_square structure with the specified position, size, and color.
 */
t_square	init_square(int x, int y, int size, int color)
{
	t_square	square;

	square.x = x;
	square.y = y;
	square.size = size;
	square.color = color;
	return (square);
}
