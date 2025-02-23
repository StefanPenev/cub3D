/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:53:19 by stefan            #+#    #+#             */
/*   Updated: 2025/02/23 12:05:48 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"
#include "draw.h"

int	is_wall(t_ctrl *ctrl, float x, float y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x / TILE_SIZE);
	map_y = (int)(y / TILE_SIZE);
	if (map_x < 0 || map_x >= WIDTH || map_y < 0 || map_y >= HEIGHT)
		return (1);
	return (ctrl->map.full_map[map_y][map_x] == '1');
}

bool	enemy_is_dead(const t_enemy *enemy)
{
	if (enemy->is_dead)
		return (true);
	return (false);
}

void	enemy_fill_data(t_enemy_draw *ed, t_player *pl, t_enemy *en)
{
	ed->dx = en->x - pl->x;
	ed->dy = en->y - pl->y;
	ed->dist = sqrtf(ed->dx * ed->dx + ed->dy * ed->dy);
	ed->angle = atan2f(ed->dy, ed->dx);
	normalize_angle(&ed->angle);
}

bool	enemy_in_fov(t_enemy_draw *ed, float pl_angle)
{
	ed->diff = ed->angle - pl_angle;
	while (ed->diff > M_PI)
		ed->diff -= 2.0f * M_PI;
	while (ed->diff < -M_PI)
		ed->diff += 2.0f * M_PI;
	if (fabsf(ed->diff) > (M_PI / 3.0f) / 2.0f)
		return (false);
	if (ed->dist < 0.0001f)
		ed->eff_dist = 0.0001f;
	else
		ed->eff_dist = ed->dist;
	return (true);
}
