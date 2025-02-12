/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hp_bar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:14:03 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/12 13:55:38 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// void you_lose(t_game *game)
// {
//     if (game->game_over)
//         return;
//     game->game_over = 1; 
//     mlx_put_image_to_window(game->mlx, game->win, game->lose_img.frames[0], 0, 0);
//     //mlx_do_sync(game->mlx);
// }


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
	static int msg_printed = 0;
	x = 10;
	y = HEIGHT - 20;
	width = WIDTH / 4;
	height = 10;
	color = 0xFF0000;
    // printf("DEBUG: HP Bar function called. HP: %d, Fight: %d, Enemy Shoot: %d\n",
    //        game->player.hp,
    //        game->player.fight.fight_started,
    //        game->player.fight.enemy_shoot);
	if (game->fight.lose_flag)
	{
		return;
	}
	hp_width = (width * game->player.hp) / 100;
	if (game->fight.fight_started && game->fight.enemy_shoot)
	{
		//printf("DEBUG: Enemy deals damage! HP before: %d\n", game->player.hp);
		game->player.hp -= ENEMY_DAMAGE;
		printf("Enemy deals damage! Your HP: %d\n", game->player.hp);
		game->fight.enemy_shoot = 0; //fight simulator
		if (game->player.hp <= 0)
		{
			printf(COLOR_RED "You lose! Try again.\n" COLOR_RESET);
			game->player.hp = 0;
			game->fight.lose_flag = 1;
			//you_lose(game);
		}
	}
    //fight simulator
	//if (game->player.hp == 0)
	// {
	// 	if (game->fight.lose_flag)
	// 		return;
	// 	game->fight.lose_flag = 1;
		//game->fight.fight_started = 0;
		//printf("game->player.hp = 0\n");
	//}
    //fight simulator end
	if (!game->fight.fight_started && game->player.hp < PLAYER_HP)
	{
		if (msg_printed == 0)
		{
		msg_printed = 1;
		printf(COLOR_GREEN "Restoring HP...\n" COLOR_RESET);
		}
		//printf("DEBUG: Restoring HP... Timer: %.2f\n", restore_timer);
		restore_timer -= delta_time * 10; //increase this value if want to restore 
        //hp faster
		if (restore_timer <= 0)
		{
			game->player.hp += 10; //increase this value if want to restore bigger count of hp
			//printf("DEBUG: HP restored! New HP: %d\n", game->player.hp);
			if (game->player.hp >= PLAYER_HP)
			{
				printf(COLOR_GREEN "Your HP fully restored!\n" COLOR_RESET);
				msg_printed = 0;
				game->player.hp = PLAYER_HP;
			}
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

