/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/02/11 00:36:15 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->img_data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = color;
}

static void	clear_image(t_game *game)
{
	memset(game->img_data, 0, HEIGHT * game->size_line);
}

void	draw_square(t_square sq, t_game *game)
{
	int	i;
	int	j;
	int	x_pos;
	int	y_pos;

	j = 0;
	while (j < sq.size)
	{
		y_pos = sq.y + j;
		if (y_pos >= 0 && y_pos < HEIGHT)
		{
			i = 0;
			while (i < sq.size)
			{
				x_pos = sq.x + i;
				if (x_pos >= 0 && x_pos < WIDTH)
					put_pixel(x_pos, y_pos, sq.color, game);
				i++;
			}
		}
		j++;
	}
}

void	draw_map(t_map *mapp, t_game *game)
{
	int		x;
	int		y;
	char	**map;
	int		block_size;
	int		color;

	color = MAP_COLOR;
	block_size = TILE_SIZE;
	map = mapp->full_map;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
			{
				draw_square(init_square(x * block_size, y * block_size,
						block_size, color), game);
			}
			x++;
		}
		y++;
	}
}

void	draw_cross(t_game *game)
{
	int	center_x;
	int	center_y;
	int	size;
	int	color;
	int	i;

	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	size = 10;
	color = 0x00FF00;
	i = -size;
	while (i <= size)
	{
		put_pixel(center_x + i, center_y, color, game);
		i++;
	}
	i = -size;
	while (i <= size)
	{
		put_pixel(center_x, center_y + i, color, game);
		i++;
	}
}

void	draw_weapon(t_texture *texture, t_game *game, int scale,
		int recoil_offset)
{
	int	color;
	int	screen_x;
	int	screen_y;
	int	y;
	int	x;
	int	weapon_x;
	int	weapon_y;
	int	i;
	int	j;

	weapon_x = (WIDTH / 2) - ((TEX_WIDTH * scale) / 2.3);
	// change divider to move weapon left or right
	weapon_y = HEIGHT - (TEX_HEIGHT * scale) + recoil_offset;
	y = 0;
	while (y < TEX_HEIGHT)
	{
		x = 0;
		while (x < TEX_WIDTH)
		{
			color = get_texture_color(texture, x, y);
			if (color != 0x000000)
			 {
				i = 0;
				while (i < scale)
				{
					j = 0;
					while (j < scale)
					{
						screen_x = weapon_x + (x * scale) + i;
						screen_y = weapon_y + (y * scale) + j;
						if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0
							&& screen_y < HEIGHT)
							put_pixel(screen_x, screen_y, color, game);
						j++;
					}
					i++;
				}
			}
			x++;
		}
		y++;
	}
}

void	choose_weapon(t_game *game)
{
	int	scale;
	int	recoil_offset;

	recoil_offset = 0;
	scale = 3;
	if (game->is_shooting)
	{
		recoil_offset = 10;
		draw_weapon(&game->weapon_shoot, game, scale, recoil_offset);
		game->shoot_ac++;
		if (game->shoot_ac >= 50) //change this value to choose how long shooting frame should be on the screen
		{
			//recoil_offset = 0;
			game->is_shooting = 0;
			game->shoot_ac = 0;
		}
	}
	else
	{
		draw_weapon(&game->weapon_idle, game, scale, recoil_offset);
	}
}

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

void draw_enemy(t_ctrl *ctrl, t_player *player, t_enemy *enemy)
{
    // Calculate enemy's world position relative to the player.
    float enemy_world_x = enemy->x;
    float enemy_world_y = enemy->y;
    float dx = enemy_world_x - player->x;
    float dy = enemy_world_y - player->y;
    float distance = sqrtf(dx * dx + dy * dy);
    float enemy_angle = atan2f(dy, dx);
    
    // Calculate angle difference between enemy and player's view.
    float angle_diff = enemy_angle - player->angle;
    while (angle_diff > M_PI)
        angle_diff -= 2 * M_PI;
    while (angle_diff < -M_PI)
        angle_diff += 2 * M_PI;

    // If the enemy is outside the player's field of view, do not draw.
    float half_fov = (M_PI / 3) / 2;
    if (fabs(angle_diff) > half_fov)
        return;

    // Compute the perpendicular distance to the enemy.
    //float enemy_perp_distance = distance * cos(angle_diff);
    float enemy_perp_distance = distance;
    float fov = M_PI / 3;
    float dist_plane = (WIDTH / 2) / tanf(fov / 2);

    // Calculate the sprite's size and screen position.
    int sprite_height = abs((int)((TILE_SIZE / enemy_perp_distance) * dist_plane));
    int sprite_width = sprite_height;
    int sprite_screen_x = (int)((WIDTH / 2) + (tanf(angle_diff) * dist_plane)) - sprite_width / 2;
    int sprite_screen_y = (HEIGHT / 2) - sprite_height / 2;

    // Raycasting: Check if a wall blocks the enemy's view.
    float ray_x = player->x;
    float ray_y = player->y;
    float step_x = dx / distance * TILE_SIZE;
    float step_y = dy / distance * TILE_SIZE;
    float ray_dist = 0;
    while (ray_dist < distance)
    {
        ray_x += step_x;
        ray_y += step_y;
        ray_dist += TILE_SIZE;
        if (is_wall(ctrl, ray_x, ray_y))
            return;
    }

    // Animation 
    // if (ctrl->game->enemy.frames_count > 1)
	// {
    // 	enemy->frame_time += compute_delta_time();
    // 	if (enemy->frame_time >= enemy->frame_duration)
    // 	{
    //     	enemy->frame = (enemy->frame + 1) % ctrl->game->enemy.frames_count;
    //     	enemy->frame_time = 0;
    // 	}
	// }	
    // ------------------------------------------------------------

    // Render the enemy sprite (or animated texture) onto the screen.
    for (int x = 0; x < sprite_width; x++)
    {
        int screen_x = sprite_screen_x + x;
        if (screen_x < 0 || screen_x >= WIDTH)
            continue;
        // Use the zbuffer to skip pixels where a wall is closer than the enemy.
        if (ctrl->game->zbuffer && (enemy_perp_distance > ctrl->game->zbuffer[screen_x]))
            continue;
        for (int y = 0; y < sprite_height; y++)
        {
            int screen_y = sprite_screen_y + y;
            if (screen_y < 0 || screen_y >= HEIGHT)
                continue;
            // Map the current screen pixel to texture coordinates.
            int tex_x = x * TEX_WIDTH / sprite_width;
            int tex_y = y * TEX_HEIGHT / sprite_height;
            // Get the pixel color from the enemy texture using the current frame.
            int color = get_texture_color(&ctrl->game->enemy, tex_x, tex_y);
            // Assume that a color value of 0 is transparent.
            if (color != 0)
                put_pixel(screen_x, screen_y, color, ctrl->game);
        }
    }
}


int	draw_loop(t_ctrl *ctrl)
{
	float	fov;
	float	start_angle;
	float	angle_step;
	double	delta_time;
	size_t	i;

	i = 0;
	ctrl->anim.fc++;
	// printf("DEBUG: ctrl->anim.fc: %zu\n", ctrl->anim.fc);
	if (ctrl->anim.fc >= TIME_SPEED)
	{
		ctrl->anim.fc = 0;
		ctrl->anim.ac++;
		// printf("DEBUG: frame index: %zu\n", ctrl->anim.ac);
	}
	if (ctrl->anim.ac >= MAX_FRAMES)
	{
		ctrl->anim.ac = 0;
	}
	delta_time = compute_delta_time();
	// printf("DEBUG: delta_time = %f\n", delta_time);
	// door
	while (i < ctrl->map.doors_counter)
	{
		if (ctrl->map.doors[i].state == DOOR_OPENING || ctrl->map.doors[i].state == DOOR_OPEN)
		{
			update_doors(&ctrl->map.doors[i], ctrl, delta_time);
		}
		i++;
	}
	// door
	clear_image(ctrl->game);
	move_player(ctrl, delta_time);
	if (ctrl->game->debug)
		draw_debug(ctrl);
	else
	{
		fov = M_PI / 3.0f;
		start_angle = ctrl->game->player.angle - (fov / 2.0f);
		angle_step = fov / WIDTH;
		handle_rays(ctrl, start_angle, angle_step);
		i = 0;
		while (i < ctrl->map.enemies_counter)
		{
			draw_enemy(ctrl, &ctrl->game->player, &ctrl->map.enemies[i]);
			i++;
		}
	}
	draw_cross(ctrl->game);
	draw_minimap(&ctrl->map, ctrl->game);
	choose_weapon(ctrl->game);
	mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win, ctrl->game->img,
		0, 0);
	return (0);
}
