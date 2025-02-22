/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/02/22 14:22:11 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static void	manage_enemies(t_ctrl *ctrl, double dt)
{
	size_t	i;

	i = 0;
	while (i < ctrl->map.enemies_counter)
	{
		update_enemy_state(&ctrl->map.enemies[i], &ctrl->game->player,
			ctrl, dt);
		ctrl->game->enemy.current_frame = ctrl->map.enemies[i].frame;
		if (check_enemy_visibility(&ctrl->map.enemies[i], ctrl))
			draw_enemy(ctrl, &ctrl->game->player, &ctrl->map.enemies[i]);
		i++;
	}
}

static void	draw_view(t_ctrl *ctrl, double dt)
{
	float	fov;
	float	start;
	float	step;

	if (ctrl->game->debug)
		draw_debug(ctrl);
	else
	{
		fov = M_PI / 3.0f;
		start = ctrl->game->player.angle - (fov / 2.0f);
		step = fov / WIDTH;
		handle_rays(ctrl, start, step);
		manage_enemies(ctrl, dt);
	}
}

static void	draw_interface(t_ctrl *ctrl, double dt)
{
	draw_minimap(ctrl);
	draw_controls_button(ctrl->game);
	if (ctrl->show_controls)
		draw_controls(ctrl->game);
	choose_weapon(ctrl->game);
	draw_hp_bar(ctrl->game, dt);
	draw_crosshair(ctrl->game);
}

static int	handle_end_game(t_ctrl *ctrl)
{
	if (ctrl->game->fight.lose_flag)
	{
		mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win,
			ctrl->game->lose_img.frames[0], 0, 0);
		return (0);
	}
	else if (ctrl->game->fight.win_flag)
	{
		mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win,
			ctrl->game->win_img.frames[0], 0, 0);
		return (0);
	}
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win,
		ctrl->game->img, 0, 0);
	return (0);
}

int	draw_loop(t_ctrl *ctrl)
{
	double	dt;

	animate(ctrl);
	dt = compute_delta_time();
	animate_doors(ctrl, dt);
	clear_image(ctrl->game);
	draw_skybox(ctrl->game);
	move_player(ctrl, dt);
	draw_view(ctrl, dt);
	draw_interface(ctrl, dt);
	return (handle_end_game(ctrl));
}
