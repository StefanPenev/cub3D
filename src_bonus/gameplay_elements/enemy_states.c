/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_states.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 10:23:59 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 11:51:55 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

#define THRESHOLD 200.0f
#define FRAME_DELAY 0.4f

static void	handle_enemy_idle(t_enemy *enemy, double distance)
{
	if (distance < THRESHOLD)
	{
		enemy->state = ENEMY_TRIGGERED;
		enemy->frame = 1;
		enemy->frame_time = 0;
	}
}

static void	handle_enemy_triggered(t_enemy *enemy, float delta_time,
		float transition_delay)
{
	enemy->frame_time += delta_time;
	if (enemy->frame_time >= FRAME_DELAY)
	{
		enemy->frame_time = 0;
		if (enemy->frame < 4)
			enemy->frame++;
		else
			enemy->frame_time = -transition_delay;
	}
	if (enemy->frame_time >= 0)
	{
		enemy->state = ENEMY_ACTIVE;
		enemy->frame = 1;
	}
}

static void	handle_enemy_active(t_enemy *enemy, t_ctrl *ctrl, double distance,
		float delta_time)
{
	int	enemy_visible;

	enemy_visible = 0;
	if (distance >= THRESHOLD)
		enemy->state = ENEMY_RETURN_IDLE;
	else
	{
		enemy->frame_time += delta_time;
		if (enemy->frame_time >= FRAME_DELAY)
		{
			enemy->frame_time = 0;
			if (enemy->frame == 4)
				enemy->frame = 5;
			else
				enemy->frame = 4;
		}
		if (check_enemy_visibility(enemy, ctrl))
		{
			enemy_visible = 1;
			enemy_attack(ctrl, enemy);
		}
		ctrl->game->fight.fight_started = enemy_visible;
	}
}

static void	handle_enemy_return_state(t_enemy *enemy, t_ctrl *ctrl)
{
	enemy->state = ENEMY_IDLE;
	enemy->frame = 0;
	enemy->frame_time = 0;
	ctrl->game->fight.fight_started = 0;
}

void	update_enemy_state(t_enemy *enemy, t_player *player, t_ctrl *ctrl,
	double delta_time)
{
	float	distance;
	float	transition_delay;

	if (enemy->is_dead)
		return ;
	distance = compute_distance(enemy->x, enemy->y, player->x, player->y);
	transition_delay = 0.5f;
	if (enemy->state == ENEMY_IDLE)
		handle_enemy_idle(enemy, distance);
	else if (enemy->state == ENEMY_TRIGGERED)
		handle_enemy_triggered(enemy, delta_time, transition_delay);
	else if (enemy->state == ENEMY_ACTIVE)
		handle_enemy_active(enemy, ctrl, distance, delta_time);
	else if (enemy->state == ENEMY_RETURN_IDLE)
		handle_enemy_return_state(enemy, ctrl);
}
