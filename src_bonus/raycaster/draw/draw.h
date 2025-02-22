/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:35:16 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 18:15:42 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

typedef struct s_weapon
{
	int		offset_x;
	int		offset_y;
	int		scale;
	int		base_x;
	int		base_y;
}			t_weapon;

typedef struct s_floorcalc
{
	float	row_dist;
	float	weight;
	float	floor_x;
	float	floor_y;
	int		tex_x;
	int		tex_y;
	int		color;
}			t_floorcalc;

typedef struct s_skydraw
{
	int		tex_y;
	float	offset;
}			t_skydraw;

typedef struct s_enemy_draw
{
	float	dx;
	float	dy;
	float	dist;
	float	angle;
	float	diff;
	float	eff_dist;
	float	fov;
	float	dist_plane;
	int		h;
	int		w;
	int		sx;
	int		sy;
}			t_enemy_draw;

bool	enemy_is_dead(const t_enemy *enemy);
int		is_wall(t_ctrl *ctrl, float x, float y);
bool	enemy_in_fov(t_enemy_draw *ed, float pl_angle);
void	enemy_fill_data(t_enemy_draw *ed, t_player *pl, t_enemy *en);

#endif