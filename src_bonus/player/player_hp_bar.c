/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hp_bar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:14:03 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/21 18:10:10 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

#define X 10
#define BAR_HEIGHT 20

// Handles the player taking damage from an enemy attack.
// Reduces player HP by ENEMY_DAMAGE, prints the updated HP.
// If HP drops to 0 or below, sets the lose_flag and prints a losing message.

void	get_hit(t_game *game)
{
	game->player.hp -= ENEMY_DAMAGE;
	printf(COLOR_YELLOW "Enemy deals damage! Your HP: %d\n" COLOR_RESET,
		game->player.hp);
	game->fight.enemy_shoot = 0;
	if (game->player.hp <= 0)
	{
		game->player.hp = 0;
		printf(COLOR_RED "You lose! Try again.\n" COLOR_RESET);
		game->fight.lose_flag = 1;
	}
}

// Draws the outline (frame) of the health bar.
// The frame is drawn in a gray color around the health bar area.

void	draw_bar_frame(int width, int y, t_game *game)
{
	int	frame_color;
	int	j;
	int	i;

	i = 0;
	frame_color = 0xA6A6A6;
	while (i < width)
	{
		put_pixel(X + i, y, frame_color, game);
		put_pixel(X + i, y + BAR_HEIGHT - 1, frame_color, game);
		i++;
	}
	j = 0;
	while (j < BAR_HEIGHT)
	{
		put_pixel(X, y + j, frame_color, game);
		put_pixel(X + width - 1, y + j, frame_color, game);
		j++;
	}
}

// Draws the background of the HP bar.
// Fills the HP bar area with a dark color representing empty HP.

void	draw_bar_background(int width, int y, t_game *game)
{
	int	i;
	int	j;
	int	background_color;

	i = 0;
	background_color = 0x444444;
	while (i < width)
	{
		j = 0;
		while (j < BAR_HEIGHT)
		{
			put_pixel(X + i, y + j, background_color, game);
			j++;
		}
		i++;
	}
}

// Draws the red section of the HP bar representing current HP.
// Renders a red rectangle proportional to the player’s remaining HP.

void	draw_red_bar(int hp_width, int y, t_game *game)
{
	int	red;
	int	i;
	int	j;

	i = 0;
	red = 0xFF0000;
	while (i < hp_width)
	{
		j = 0;
		while (j < BAR_HEIGHT)
		{
			put_pixel(X + i, y + j, red, game);
			j++;
		}
		i++;
	}
}

// Gradually restores player’s HP when not in combat.
// Displays messages during restoration and resets values when HP is full.

void	restore_hp(t_game *game, double delta_time)
{
	static int		msg_printed = 0;
	static double	restore_timer = 5.0;

	if (msg_printed == 0)
	{
		msg_printed = 1;
		printf(COLOR_GREEN "Restoring HP...\n" COLOR_RESET);
	}
	restore_timer -= delta_time * 10;
	if (restore_timer <= 0)
	{
		game->player.hp += 10;
		if (game->player.hp >= PLAYER_HP)
		{
			printf(COLOR_GREEN "Your HP fully restored!\n" COLOR_RESET);
			msg_printed = 0;
			game->player.hp = PLAYER_HP;
		}
		restore_timer = 5.0;
	}
}
