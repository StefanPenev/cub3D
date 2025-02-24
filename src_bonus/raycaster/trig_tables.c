/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_tables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:01:17 by stefan            #+#    #+#             */
/*   Updated: 2025/02/24 09:44:25 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static t_trig_tables	*allocate_trig_tables(void)
{
	t_trig_tables	*tables;

	tables = malloc(sizeof(t_trig_tables));
	if (!tables)
	{
		printf("Error: Memory allocation failed for trig tables.\n");
		exit(EXIT_FAILURE);
	}
	tables->sin_table = malloc(ANGLE_TABLE_SIZE * sizeof(double));
	tables->cos_table = malloc(ANGLE_TABLE_SIZE * sizeof(double));
	if (!tables->sin_table || !tables->cos_table)
	{
		printf("Error: Memory allocation failed for trig arrays.\n");
		free(tables->sin_table);
		free(tables->cos_table);
		free(tables);
		exit(EXIT_FAILURE);
	}
	return (tables);
}

static void	fill_trig_tables(t_trig_tables *tables)
{
	int		i;
	double	angle_rad;

	i = 0;
	while (i < ANGLE_TABLE_SIZE)
	{
		angle_rad = i * (2 * M_PI / ANGLE_TABLE_SIZE);
		tables->sin_table[i] = sin(angle_rad);
		tables->cos_table[i] = cos(angle_rad);
		i++;
	}
}

t_trig_tables	*init_trig_tables(void)
{
	t_trig_tables	*tables;

	tables = allocate_trig_tables();
	fill_trig_tables(tables);
	return (tables);
}

void	free_trig_tables(t_trig_tables *tables)
{
	if (tables)
	{
		free(tables->sin_table);
		free(tables->cos_table);
		free(tables);
	}
}

int	get_table_index(float angle)
{
	int	index;

	index = (int)(angle * (ANGLE_TABLE_SIZE / (2 * M_PI))) % ANGLE_TABLE_SIZE;
	if (index < 0)
		index += ANGLE_TABLE_SIZE;
	return (index);
}
