/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_square.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:29:21 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 14:52:51 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_square	init_square(int x, int y, int size, int color)
{
	t_square	square;

	square.x = x;
	square.y = y;
	square.size = size;
	square.color = color;
	return (square);
}
