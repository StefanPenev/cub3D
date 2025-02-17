/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hp_bar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:14:03 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/17 08:12:14 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void	get_hit(t_game *game)
{
	game->player.hp -= ENEMY_DAMAGE;
	printf(COLOR_YELLOW "Enemy deals damage! Your HP: %d\n" COLOR_RESET,
		game->player.hp);
	game->fight.enemy_shoot = 0;
	if (game->player.hp <= 0)
	{
		printf(COLOR_RED "You lose! Try again.\n" COLOR_RESET);
		game->player.hp = 0;
		game->fight.lose_flag = 1;
	}
}

// void restore_hp()
// {
// }

// void	draw_hp_bar(t_game *game, double delta_time)
// {
// 	static double	restore_timer = 5.0;
// 	int				x;
// 	int				y;
// 	int				width;
// 	int				height;
// 	int				color;
// 	int				hp_width;
// 	int				i;
// 	int				j;
// 	static int		msg_printed = 0;

// 	x = 10;
// 	y = HEIGHT - 20;
// 	width = WIDTH / 4;
// 	height = 10;
// 	color = 0xFF0000;
// 	if (game->fight.lose_flag)
// 		return ;
// 	hp_width = (width * game->player.hp) / 100;
// 	if (game->fight.fight_started && game->fight.enemy_shoot)
// 		get_hit(game);
// 	if (!game->fight.fight_started && game->player.hp < PLAYER_HP)
// 	{
// 		if (msg_printed == 0)
// 		{
// 			msg_printed = 1;
// 			printf(COLOR_GREEN "Restoring HP...\n" COLOR_RESET);
// 		}
// 		restore_timer -= delta_time * 10;
// 		// increase this value if want to restore
// 		// hp faster
// 		if (restore_timer <= 0)
// 		{
// 			game->player.hp += 10;
// 			// increase this value if want to restore bigger count of hp
// 			if (game->player.hp >= PLAYER_HP)
// 			{
// 				printf(COLOR_GREEN "Your HP fully restored!\n" COLOR_RESET);
// 				msg_printed = 0;
// 				game->player.hp = PLAYER_HP;
// 			}
// 			restore_timer = 5.0;
// 		}
// 	}
// 	for (i = 0; i < hp_width; i++)
// 	{
// 		for (j = 0; j < height; j++)
// 		{
// 			put_pixel(x + i, y + j, color, game);
// 		}
// 	}
// }

void	draw_hp_bar(t_game *game, double delta_time)
{
	static double	restore_timer = 5.0;
	int				x;
	int				y;
	int				width;
	int				height;
	int				red;
	int				hp_width;
	int				i;
	int				j;
	static int		msg_printed = 0;
	int				frame_color;
	int				background_color;

	x = 10;
	y = HEIGHT - 30;
	width = WIDTH / 4;
	height = 20;
	red = 0xFF0000;
	frame_color = 0xA6A6A6;
	background_color = 0x444444;

	if (game->fight.lose_flag)
		return ;
	hp_width = (width * game->player.hp) / 100;
	if (game->fight.fight_started && game->fight.enemy_shoot)
		get_hit(game);
	if (!game->fight.fight_started && game->player.hp < PLAYER_HP)
	{
		if (msg_printed == 0)
		{
			msg_printed = 1;
			printf(COLOR_GREEN "Restoring HP...\n" COLOR_RESET);
		}
		restore_timer -= delta_time * 10;
		// increase this value if want to restore hp faster
		if (restore_timer <= 0)
		{
			game->player.hp += 10;
			// increase this value if want to restore bigger count of hp
			if (game->player.hp >= PLAYER_HP)
			{
				printf(COLOR_GREEN "Your HP fully restored!\n" COLOR_RESET);
				msg_printed = 0;
				game->player.hp = PLAYER_HP;
			}
			restore_timer = 5.0;
		}
	}
	// Draw the background of the full health bar area
	i = 0;
	while (i < width)
	{
		j = 0;
		while (j < height)
		{
			put_pixel(x + i, y + j, background_color, game);
			j++;
		}
		i++;
	}
	// Draw the current hp bar in red
	i = 0;
	while (i < hp_width)
	{
		j = 0;
		while (j < height)
		{
			put_pixel(x + i, y + j, red, game);
			j++;
		}
		i++;
	}
	// Draw the frame (outline)
	// Top and bottom borders
	i = 0;
	while (i < width)
	{
		put_pixel(x + i, y, frame_color, game);
		put_pixel(x + i, y + height - 1, frame_color, game);
		i++;
	}
	// Left and right borders
	j = 0;
	while (j < height)
	{
		put_pixel(x, y + j, frame_color, game);
		put_pixel(x + width - 1, y + j, frame_color, game);
		j++;
	}
}
