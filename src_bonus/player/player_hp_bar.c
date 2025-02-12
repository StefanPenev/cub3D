/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hp_bar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:14:03 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/12 11:53:38 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	draw_hp_bar(t_game *game, double delta_time)
{
	static double	restore_timer = 5.0;

	int				x;
	int				y;
	int				width;
	int				height;
	int				color;
	int				hp_width;
	int				i;
	int				j;

	x = 10;
	y = HEIGHT - 20;
	width = WIDTH / 4;
	height = 10;
	color = 0xFF0000;
    // printf("DEBUG: HP Bar function called. HP: %d, Fight: %d, Enemy Shoot: %d\n",
    //        game->player.hp,
    //        game->player.fight.fight_started,
    //        game->player.fight.enemy_shoot);
	hp_width = (width * game->player.hp) / 100;
	if (game->fight.fight_started && game->fight.enemy_shoot)
	{
		printf("DEBUG: Enemy deals damage! HP before: %d\n", game->player.hp);
		game->player.hp -= ENEMY_DAMAGE;
		if (game->player.hp < 0)
			game->player.hp = 0;
		printf("DEBUG: HP after: %d\n", game->player.hp);
		game->fight.enemy_shoot = 0; //fight simulator
	}
    //fight simulator
	if (game->player.hp == 0)
	{
		game->fight.fight_started = 0;
		//printf("game->player.hp = 0\n");
	}
    //fight simulator end
	if (!game->fight.fight_started && game->player.hp < PLAYER_HP)
	{
		//printf("DEBUG: Restoring HP... Timer: %.2f\n", restore_timer);
		restore_timer -= delta_time * 10; //increase this value if want to restore 
        //hp faster
		if (restore_timer <= 0)
		{
			game->player.hp += 10; //increase this value if want to restore bigger count of hp
			//printf("DEBUG: HP restored! New HP: %d\n", game->player.hp);
			if (game->player.hp > PLAYER_HP)
				game->player.hp = PLAYER_HP;
			restore_timer = 5.0;
		}
	}
	for (i = 0; i < hp_width; i++)
	{
		for (j = 0; j < height; j++)
		{
			put_pixel(x + i, y + j, color, game);
		}
	}
}

