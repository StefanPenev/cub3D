/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_attack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:05:08 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/14 14:14:03 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

void enemy_take_damage(t_game *game, t_enemy *enemy)
{
    if (!enemy)
    {
        printf(COLOR_RED "Error: enemy_take_damage() received NULL pointer!\n" COLOR_RESET);
        return;
    }

    enemy->enemy_hp -= PLAYER_DAMAGE;
    if (enemy->enemy_hp <= 0)
    {
        enemy->enemy_hp = 0;
        enemy->is_dead = 1;
		enemy->frame = 6;  
		game->fight.fight_started = 0;
        printf(COLOR_GREEN "Enemy is dead!\n" COLOR_RESET);
    }
}

t_enemy *find_enemy_by_position(t_ctrl *ctrl, int x, int y)
{
    size_t i;

    printf(COLOR_YELLOW "[DEBUG] Searching for enemy at (%d, %d)\n" COLOR_RESET, x, y);

    for (i = 0; i < ctrl->map.enemies_counter; i++)
    {
        int enemy_map_x = (int)(ctrl->map.enemies[i].x / TILE_SIZE);
        int enemy_map_y = (int)(ctrl->map.enemies[i].y / TILE_SIZE);

        printf(COLOR_YELLOW "[DEBUG] Checking enemy at (%.2f, %.2f) -> Map cell: (%d, %d)\n" COLOR_RESET,
               ctrl->map.enemies[i].x, ctrl->map.enemies[i].y, enemy_map_x, enemy_map_y);

        // 🔹 Проверяем попадание в клетку карты
        if (enemy_map_x == x && enemy_map_y == y)
        {
            printf(COLOR_GREEN "[DEBUG] Enemy FOUND at (%d, %d)!\n" COLOR_RESET, x, y);
            return &ctrl->map.enemies[i]; // Враг найден!
        }
    }

    printf(COLOR_RED "[ERROR] Enemy NOT found at (%d, %d)\n" COLOR_RESET, x, y);
    return NULL; // Враг не найден
}


float calculate_distance(float x1, float y1, float x2, float y2)
{
    int tile_size = TILE_SIZE;  // Размер тайла (если у тебя другой — измени)

    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrtf((dx * dx) + (dy * dy));

    return distance / tile_size;  // Переводим из пикселей в тайлы
}



void cast_ray(t_ctrl *ctrl, t_raycast *ray, float start_x, float start_y, float angle)
{

    int tile_size = TILE_SIZE;  

    ray->ray_dir_x = cos(angle);
    ray->ray_dir_y = sin(angle);
    ray->map_x = (int)(start_x / tile_size);
    ray->map_y = (int)(start_y / tile_size);
    ray->hit = 0;

    printf(COLOR_YELLOW "[DEBUG] Casting ray from (%.2f, %.2f) | Map cell: (%d, %d) | Dir: (%.2f, %.2f)\n" COLOR_RESET,
           start_x, start_y, ray->map_x, ray->map_y, ray->ray_dir_x, ray->ray_dir_y);

    float delta_dist_x = fabs(1 / ray->ray_dir_x);
    float delta_dist_y = fabs(1 / ray->ray_dir_y);

    while (!ray->hit)
    {

        if (ray->map_x < 0 || ray->map_x >= (int)ctrl->map.columns ||
            ray->map_y < 0 || ray->map_y >= (int)ctrl->map.rows)
        {
            printf(COLOR_RED "[ERROR] Ray exited map bounds at (%d, %d)\n" COLOR_RESET, ray->map_x, ray->map_y);
            ray->hit = 1;
            return;
        }


        if (delta_dist_x < delta_dist_y)
        {
            ray->map_x += (ray->ray_dir_x > 0) ? 1 : -1;
            delta_dist_x += fabs(1 / ray->ray_dir_x);
        }
        else
        {
            ray->map_y += (ray->ray_dir_y > 0) ? 1 : -1;
            delta_dist_y += fabs(1 / ray->ray_dir_y);
        }

        printf(COLOR_YELLOW "[DEBUG] Ray moved to: (%d, %d)\n" COLOR_RESET, ray->map_x, ray->map_y);

        char hit_object = ctrl->map.full_map[ray->map_y][ray->map_x];

        if (hit_object == '1') 
        {
            ray->hit = 1;
            printf(COLOR_YELLOW "[DEBUG] Ray hit a wall at (%d, %d)\n" COLOR_RESET, ray->map_x, ray->map_y);
        }

        if (hit_object == 'M') 
        {
            ray->hit = 1;
            printf(COLOR_GREEN "[DEBUG] Ray hit an enemy at (%d, %d)\n" COLOR_RESET, ray->map_x, ray->map_y);
        }
    }
}

void shoot(t_ctrl *ctrl)
{
    t_raycast ray;
    t_enemy *enemy;
    float enemy_distance;


    float cross_x = ctrl->game->player.x + cos(ctrl->game->player.angle) * 1.0;
    float cross_y = ctrl->game->player.y + sin(ctrl->game->player.angle) * 1.0;

    printf(COLOR_YELLOW "[DEBUG] Player Position: (%.2f, %.2f) | Angle: %.2f\n" COLOR_RESET,
           ctrl->game->player.x, ctrl->game->player.y, ctrl->game->player.angle);
    printf(COLOR_YELLOW "[DEBUG] Crosshair Position: (%.2f, %.2f)\n" COLOR_RESET, cross_x, cross_y);

    cast_ray(ctrl, &ray, cross_x, cross_y, ctrl->game->player.angle);

    printf(COLOR_YELLOW "[DEBUG] Ray Stopped At: (%d, %d) | Hit: %d\n" COLOR_RESET, ray.map_x, ray.map_y, ray.hit);


    if (ray.map_x < 0 || ray.map_x >= (int)ctrl->map.columns ||
        ray.map_y < 0 || ray.map_y >= (int)ctrl->map.rows)
    {
        printf(COLOR_RED "[ERROR] Ray out of bounds: (%d, %d)\n" COLOR_RESET, ray.map_x, ray.map_y);
        return;
    }

    if (ray.hit && ctrl->map.full_map[ray.map_y][ray.map_x] == 'M')
    {
        enemy = find_enemy_by_position(ctrl, ray.map_x, ray.map_y);

        if (!enemy)
        {
            printf(COLOR_RED "[ERROR] Enemy not found at (%d, %d)\n" COLOR_RESET, ray.map_x, ray.map_y);
            return;
        }

        printf(COLOR_YELLOW "[DEBUG] Enemy Found at: (%.2f, %.2f) | HP: %d | Dead: %d\n" COLOR_RESET,
               enemy->x, enemy->y, enemy->enemy_hp, enemy->is_dead);


        if (enemy->is_dead)
        {
            printf(COLOR_RED "[INFO] Enemy already dead!\n" COLOR_RESET);
            return;
        }


        enemy_distance = calculate_distance(ctrl->game->player.x, ctrl->game->player.y, enemy->x, enemy->y);
        printf(COLOR_YELLOW "[DEBUG] Distance to Enemy: %.2f tiles\n" COLOR_RESET, enemy_distance);

        if (enemy_distance > 2.0)
        {
            printf(COLOR_RED "[INFO] Miss! Enemy is too far (%.2f tiles)\n" COLOR_RESET, enemy_distance);
            return;
        }

        enemy_take_damage(ctrl->game, enemy);
        printf(COLOR_GREEN "[INFO] Hit enemy! HP left: %d\n" COLOR_RESET, enemy->enemy_hp);
    }
    else
    {
        printf(COLOR_RED "[INFO] Miss! No enemy at ray destination.\n" COLOR_RESET);
    }
}


// void	enemy_take_damage(t_enemy *enemy)
// {
// 	enemy->enemy_hp -= PLAYER_DAMAGE;
// 	if (enemy->enemy_hp <= 0)
// 	{
// 		enemy->enemy_hp = 0;
// 		enemy->is_dead = 1;
// 		printf(COLOR_GREEN "Enemy is dead!\n" COLOR_RESET);
// 	}
// }

// 	int	is_enemy_hit(t_game *game, t_enemy *enemy, float ray_dir_x,
// 			float ray_dir_y)
// 	{
// 		float	enemy_vec_x;
// 		float	enemy_vec_y;
// 		float	dot_product;
// 		float	enemy_distance;
// 		float	angle_diff;

// 		enemy_vec_x = enemy->x - game->player.x;
// 		enemy_vec_y = enemy->y - game->player.y;
// 		dot_product = (enemy_vec_x * ray_dir_x) + (enemy_vec_y * ray_dir_y);
// 		if (dot_product < 0)
// 			return (0);
// 		enemy_distance = sqrt(enemy_vec_x * enemy_vec_x + enemy_vec_y
// 				* enemy_vec_y);
// 		angle_diff = acos(dot_product / enemy_distance);
// 		return (angle_diff < 0.1);
// 	}

// 	float calculate_distance(float x1, float y1, float x2, float y2)
// 	{
// 		float dx = x2 - x1;
// 		float dy = y2 - y1;
// 		return (sqrtf((dx * dx) + (dy * dy)));
// 	}

	

// 	void	shoot(t_ctrl *ctrl)
// 	{
// 		float	ray_dir_x;
// 		float	ray_dir_y;
// 		t_enemy	*closest_enemy;
// 		t_enemy	*enemy;
// 		float	enemy_distance;
// 		float	ray_angle;
// 		float	closest_distance;
// 		size_t	i;
	
// 		i = 0;
// 		ray_angle = ctrl->game->player.angle;
// 		ray_dir_x = cos(ray_angle);
// 		ray_dir_y = sin(ray_angle);
// 		closest_enemy = NULL;
// 		closest_distance = 9999999; 
	
// 		while (i < ctrl->map.enemies_counter)
// 		{
// 			enemy = &ctrl->map.enemies[i];

// 			if (is_enemy_hit(ctrl->game, enemy, ray_dir_x, ray_dir_y))
// 			{
// 				enemy_distance = calculate_distance(ctrl->game->player.x,
// 						ctrl->game->player.y, enemy->x, enemy->y);

// 				if (enemy->is_dead)
// 				{
// 					i++;
// 					continue;
// 				}

// 				if (enemy_distance < closest_distance)
// 				{
// 					closest_distance = enemy_distance;
// 					closest_enemy = enemy;
// 				}
// 			}
// 			i++;
// 		}

// 		if (closest_enemy)
// 		{
// 			enemy_take_damage(closest_enemy);
// 			printf(COLOR_GREEN "Hit enemy!\n" COLOR_RESET);
// 		}
// 		else
// 		{
// 			printf(COLOR_RED "Miss!\n" COLOR_RESET);
// 		}
// 	}
	