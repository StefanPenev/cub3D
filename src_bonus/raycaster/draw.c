/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:07:15 by stefan            #+#    #+#             */
/*   Updated: 2025/02/21 10:43:58 by stefan           ###   ########.fr       */
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

void draw_controls_button (t_game *game)
{
    int x, y, i, j;
    size_t color;

    if (!game)// || !game->controls.img)
    {
        printf("ERROR: Controls texture not loaded!\n");
        return;
    }

    // ✅ Position at the top-right corner with 10px padding
    x = WIDTH - game->controls_button.width - 10;
    y = 10;

    // ⚡️ Debug output to ensure coordinates are correct
    // printf("Drawing controls at x: %d, y: %d, width: %d, height: %d\n",
    //        x, y, game->controls.width, game->controls.height);

    for (i = 0; i < game->controls_button.height; i++)
    {
        for (j = 0; j < game->controls_button.width; j++)
        {
            color = get_texture_color(&game->controls_button, j, i);

            // 🔄 Relax color check in case of slight background differences
            if ((color & 0xFFFFFF) != 0x000000 && (color & 0xFFFFFF) > 0x010101)
            {
                if ((x + j) >= 0 && (x + j) < WIDTH && (y + i) >= 0 && (y + i) < HEIGHT)
                    put_pixel(x + j, y + i, color, game);
            }
        }
    }
  //  printf("Controls rendering finished.\n");
}


void draw_controls(t_game *game)
{
    int x, y, i, j;
    size_t color;

    if (!game)// || !game->controls.img)
    {
        printf("ERROR: Controls texture not loaded!\n");
        return;
    }

    // ✅ Position at the top-right corner with 10px padding
    // x = WIDTH - game->controls.width - 10;
    // y = 10;
	x = (WIDTH / 2) - (game->controls.width / 2);
	y = (HEIGHT / 2) - (game->controls.height / 2);

    // ⚡️ Debug output to ensure coordinates are correct
    // printf("Drawing controls at x: %d, y: %d, width: %d, height: %d\n",
    //        x, y, game->controls.width, game->controls.height);

    for (i = 0; i < game->controls.height; i++)
    {
        for (j = 0; j < game->controls.width; j++)
        {
            color = get_texture_color(&game->controls, j, i);

            // 🔄 Relax color check in case of slight background differences
            if ((color & 0xFFFFFF) != 0x000000 && (color & 0xFFFFFF) > 0x010101)
            {
                if ((x + j) >= 0 && (x + j) < WIDTH && (y + i) >= 0 && (y + i) < HEIGHT)
                    put_pixel(x + j, y + i, color, game);
            }
        }
    }
  //  printf("Controls rendering finished.\n");
}



void	draw_crosshair(t_game *game)
{
	int		x;
	int		y;
	int		i;
	int		j;
	size_t	color;

	if (!game)
		return ;
	x = (WIDTH / 2) - (game->crosshair.width / 2);
	y = (HEIGHT / 2) - (game->crosshair.height / 2);
	i = 0;
	while (i < game->crosshair.height)
	{
		j = 0;
		while (j < game->crosshair.width)
		{
			color = get_texture_color(&game->crosshair, j, i);
			if ((color & 0xFFFFFF) != 0x000000)
			{
				put_pixel((x + 9) + j, (y + 35) + i, color, game);
				// put_pixel(x + j, y + i, color, game);
			}
			j++;
		}
		i++;
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
			if ((color & 0xFFFFFF) != 0x000000)
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
		if (game->shoot_ac >= 50)
		// change this value to choose how long shooting frame should be on the screen
		{
			// recoil_offset = 0;
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

#define THRESHOLD 200.0f

float	compute_distance(float x1, float y1, float x2, float y2)
{
	float	dx;
	float	dy;

	dx = x2 - x1;
	dy = y2 - y1;
	return (sqrtf(dx * dx + dy * dy));
}

void	update_enemy_state(t_enemy *enemy, t_player *player, t_ctrl *ctrl,
		double delta_time)
{
	float	distance;
	int		enemy_visible;
	float	transition_delay;

	if (enemy->is_dead)
	{
		//enemy->frame = 6;  
		//printf(COLOR_YELLOW "[DEBUG] Enemy at (%.2f, %.2f) is dead, rendering corpse.\n" COLOR_RESET, enemy->x, enemy->y);
		return; 
	}
	
	distance = compute_distance(enemy->x, enemy->y, player->x, player->y);
	float frame_delay = 0.4f; // Adjust this to control animation speed
	transition_delay = 0.5f;
	// Delay before switching from ENEMY_TRIGGERED to ENEMY_ACTIVE
	enemy_visible = 0;
	switch (enemy->state)
	{
	case ENEMY_IDLE:
		if (distance < THRESHOLD)
		{
			enemy->state = ENEMY_TRIGGERED;
			enemy->frame = 1;
			enemy->frame_time = 0;
		}
		break ;
	case ENEMY_TRIGGERED:
		enemy->frame_time += delta_time;
		if (enemy->frame_time >= frame_delay)
		{
			enemy->frame_time = 0;
			if (enemy->frame < 4)
				enemy->frame++;
			else
				enemy->frame_time = -transition_delay;
		}
		if (enemy->frame_time < 0)
			break ;
		enemy->state = ENEMY_ACTIVE;
		enemy->frame = 1;
		break ;
	case ENEMY_ACTIVE:
		if (distance >= THRESHOLD)
		{
			enemy->state = ENEMY_RETURN_IDLE;
		}
		else
		{
			enemy->frame_time += delta_time;
			if (enemy->frame_time >= frame_delay)
			{
				enemy->frame_time = 0;
				enemy->frame = (enemy->frame == 4) ? 5 : 4;
			}
			if (check_enemy_visibility(enemy, ctrl))
			{
				enemy_visible = 1;
				enemy_attack(ctrl, enemy);
			}
			ctrl->game->fight.fight_started = enemy_visible;
		}
		break ;
	case ENEMY_RETURN_IDLE:
		enemy->state = ENEMY_IDLE;
		enemy->frame = 0;
		enemy->frame_time = 0;
		ctrl->game->fight.fight_started = 0;
		break ;
	}
}

// void	draw_enemy(t_ctrl *ctrl, t_player *player, t_enemy *enemy)
// {
//     float	enemy_world_x;
//     float	enemy_world_y;
//     float	dx;
//     float	dy;
//     float	distance;
//     float	enemy_angle;
//     float	angle_diff;
//     float	half_fov;
//     float	enemy_perp_distance;
//     float	fov;
//     float	dist_plane;
//     int		sprite_height;
//     int		sprite_width;
//     int		sprite_screen_x;
//     int		sprite_screen_y;
//     float	ray_x;
//     float	ray_y;
//     float	step_x;
//     float	step_y;
//     float	ray_dist;
//     int		screen_x;
//     int		screen_y;
//     int		tex_x;
//     int		tex_y;
//     int		color;
//     t_texture	*sprite_texture;

//     if (enemy->is_dead)
//         return;  
//     sprite_texture = &ctrl->game->enemy;
//     enemy_world_x = enemy->x;
//     enemy_world_y = enemy->y;
//     dx = enemy_world_x - player->x;
//     dy = enemy_world_y - player->y;
//     distance = sqrtf(dx * dx + dy * dy);
//     enemy_angle = atan2f(dy, dx);
//     while (enemy_angle < 0.0f)
//         enemy_angle += 2.0f * M_PI;
//     while (enemy_angle >= 2.0f * M_PI)
//         enemy_angle -= 2.0f * M_PI;
//     angle_diff = enemy_angle - player->angle;
//     while (angle_diff > M_PI)
//         angle_diff -= 2.0f * M_PI;
//     while (angle_diff < -M_PI)
//         angle_diff += 2.0f * M_PI;
//     half_fov = (M_PI / 3.0f) / 2.0f;
//     if (fabsf(angle_diff) > half_fov)
//         return;
//     enemy_perp_distance = distance * cosf(angle_diff);
//     fov = M_PI / 3.0f;
//     dist_plane = (WIDTH / 2) / tanf(fov / 2.0f);
//     sprite_height = abs((int)((TILE_SIZE / enemy_perp_distance) * dist_plane));
//     sprite_width = sprite_height;
//     sprite_screen_x = (int)((WIDTH / 2) + tanf(angle_diff) * dist_plane)
//         - (sprite_width / 2);
//     sprite_screen_y = (HEIGHT / 2) - (sprite_height / 2);
//     ray_x = player->x;
//     ray_y = player->y;
//     step_x = (dx / distance) * (TILE_SIZE / 16.0f);
//     step_y = (dy / distance) * (TILE_SIZE / 16.0f);
//     ray_dist = 0.0f;
//     while (ray_dist < distance)
//     {
//         ray_x += step_x;
//         ray_y += step_y;
//         ray_dist += TILE_SIZE / 16.0f;
//         if (is_wall(ctrl, ray_x, ray_y))
//             return;
//     }
//     {
//         int mid_col = sprite_screen_x + sprite_width / 2;
//         if (mid_col >= 0 && mid_col < WIDTH)
//         {
//             if (ctrl->game->zbuffer &&
//                 (enemy_perp_distance > ctrl->game->zbuffer[mid_col]))
//                 return;
//         }
//     }
//     for (int x = 0; x < sprite_width; x++)
//     {
//         screen_x = sprite_screen_x + x;
//         if (screen_x < 0 || screen_x >= WIDTH)
//             continue;
//         for (int y = 0; y < sprite_height; y++)
//         {
//             screen_y = sprite_screen_y + y;
//             if (screen_y < 0 || screen_y >= HEIGHT)
//                 continue;
//             tex_x = x * TEX_WIDTH / sprite_width;
//             tex_y = y * TEX_HEIGHT / sprite_height;
//             color = get_texture_color(sprite_texture, tex_x, tex_y);
//             if (color != 0)
//                 put_pixel(screen_x, screen_y, color, ctrl->game);
//         }
//     }
// }
void draw_enemy(t_ctrl *ctrl, t_player *player, t_enemy *enemy)
{
    float enemy_world_x;
    float enemy_world_y;
    float dx;
    float dy;
    float distance;
    float enemy_angle;
    float angle_diff;
    float half_fov;
    float effective_distance;
    float fov;
    float dist_plane;
    int sprite_height;
    int sprite_width;
    int sprite_screen_x;
    int sprite_screen_y;
    float ray_x;
    float ray_y;
    float step_x;
    float step_y;
    float ray_dist;
    int screen_x;
    int screen_y;
    int tex_x;
    int tex_y;
    int color;
    t_texture *sprite_texture;

    if (enemy->is_dead)
        return;  

    sprite_texture = &ctrl->game->enemy;
    enemy_world_x = enemy->x;
    enemy_world_y = enemy->y;
    dx = enemy_world_x - player->x;
    dy = enemy_world_y - player->y;
    distance = sqrtf(dx * dx + dy * dy);

    enemy_angle = atan2f(dy, dx);
    normalize_angle(&enemy_angle);
    angle_diff = enemy_angle - player->angle;
    while (angle_diff > M_PI)
        angle_diff -= 2.0f * M_PI;
    while (angle_diff < -M_PI)
        angle_diff += 2.0f * M_PI;

    half_fov = (M_PI / 3.0f) / 2.0f;
    if (fabsf(angle_diff) > half_fov)
        return;
    effective_distance = (distance < 0.0001f) ? 0.0001f : distance;

    fov = M_PI / 3.0f;
    dist_plane = (WIDTH / 2) / tanf(fov / 2.0f);
    sprite_height = abs((int)((TILE_SIZE / effective_distance) * dist_plane));
    sprite_width = sprite_height;
    sprite_screen_x = (int)((WIDTH / 2) + (angle_diff / (fov / 2)) * (WIDTH / 2))
                        - (sprite_width / 2);
    sprite_screen_y = (HEIGHT / 2) - (sprite_height / 2);
    ray_x = player->x;
    ray_y = player->y;
    step_x = (dx / distance) * (TILE_SIZE / 16.0f);
    step_y = (dy / distance) * (TILE_SIZE / 16.0f);
    ray_dist = 0.0f;
    while (ray_dist < distance)
    {
        ray_x += step_x;
        ray_y += step_y;
        ray_dist += TILE_SIZE / 16.0f;
        if (is_wall(ctrl, ray_x, ray_y))
            return;
    }
    {
        int mid_col = sprite_screen_x + sprite_width / 2;
        if (mid_col >= 0 && mid_col < WIDTH)
        {
            if (ctrl->game->zbuffer &&
                (effective_distance > ctrl->game->zbuffer[mid_col]))
                return;
        }
    }

    // Draw enemy sprite
    for (int x = 0; x < sprite_width; x++)
    {
        screen_x = sprite_screen_x + x;
        if (screen_x < 0 || screen_x >= WIDTH)
            continue;
        for (int y = 0; y < sprite_height; y++)
        {
            screen_y = sprite_screen_y + y;
            if (screen_y < 0 || screen_y >= HEIGHT)
                continue;
            tex_x = x * TEX_WIDTH / sprite_width;
            tex_y = y * TEX_HEIGHT / sprite_height;
            color = get_texture_color(sprite_texture, tex_x, tex_y);
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

	// if (ctrl->game->game_over)
	// 	return (0);
	ctrl->anim.fc++;
	if (ctrl->anim.fc >= TIME_SPEED)
	{
		ctrl->anim.fc = 0;
		ctrl->anim.ac++;
	}
	if (ctrl->anim.ac >= MAX_FRAMES)
		ctrl->anim.ac = 0;
	delta_time = compute_delta_time();
	i = 0;
	while (i < ctrl->map.doors_counter)
	{
		if (ctrl->map.doors[i].state == DOOR_OPENING
			|| ctrl->map.doors[i].state == DOOR_OPEN)
		{
			update_doors(&ctrl->map.doors[i], ctrl, delta_time);
		}
		i++;
	}
	clear_image(ctrl->game);
	draw_skybox(ctrl->game);
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
			update_enemy_state(&ctrl->map.enemies[i], &ctrl->game->player, ctrl,
				delta_time);
			ctrl->game->enemy.current_frame = ctrl->map.enemies[i].frame;
			if (check_enemy_visibility(&ctrl->map.enemies[i], ctrl))
				draw_enemy(ctrl, &ctrl->game->player, &ctrl->map.enemies[i]);
			i++;
		}
	}
	draw_minimap(&ctrl->map, ctrl->game);
	draw_controls_button(ctrl->game);
	if (ctrl->show_controls)
	{
		draw_controls(ctrl->game);
	}
	choose_weapon(ctrl->game);
	draw_hp_bar(ctrl->game, delta_time);
	draw_crosshair(ctrl->game);
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
	else
		mlx_put_image_to_window(ctrl->game->mlx, ctrl->game->win,
			ctrl->game->img, 0, 0);
	return (0);
}
