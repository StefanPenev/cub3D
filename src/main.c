/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/16 11:29:34 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_map(char **map, size_t rows)
{
	size_t	i;

	i = 0;
	if (!map)
		return ;
	while (i < rows)
	{
		if (map[i])
			free(map[i]);
		i++;
	}
	free(map);
}

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
	{
		free_map(ctrl->map.full_map, ctrl->map.rows);
	}
	free(ctrl);
	ctrl = NULL;
}

void	check_args(int argc, char *argv[])
{
	size_t	len;

	len = 0;
	if (argc < 2)
	{
		printf("Invalid input:\nThis game requires a map.\n");
		exit(1);
	}
	if (argc > 2)
	{
		printf("Invalid input:\nThis game requires only one argument.\n");
		exit(1);
	}
	while (argv[1][len] != '\0')
	{
		len++;
	}
	if (len < 4 || ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0)
	{
		printf("Invalid input:\nMapname doesn't have a .cub extension.\n");
		exit(1);
	}
}

char	*merge_map_arr(t_ctrl *ctrl, char *line_tmp, char *map_tmp)
{
	char	*new_map_arr;

	new_map_arr = ft_strjoin(map_tmp, line_tmp);
	if (!new_map_arr)
	{
		game_cleanup(ctrl);
		free(map_tmp);
		free(line_tmp);
		exit(1);
	}
	free(map_tmp);
	free(line_tmp);
	return (new_map_arr);
}

char	*read_map(char *filename, t_ctrl *ctrl)
{
	int		file_id;
	char	*map_tmp;
	char	*line_tmp;

	map_tmp = ft_strdup("");
	if (!map_tmp)
	{
		game_cleanup(ctrl);
		exit(1);
	}
	file_id = open(filename, O_RDONLY);
	if (file_id == -1)
	{
		free(map_tmp);
		game_cleanup(ctrl);
		exit(1);
	}
	while (1)
	{
		line_tmp = get_next_line(file_id);
		if (line_tmp == NULL)
			break ;
		map_tmp = merge_map_arr(ctrl, line_tmp, map_tmp);
		ctrl->map.rows++;
	}
	close(file_id);
	printf("DEBUG: number of rows: %zu\n", ctrl->map.rows);
	return (map_tmp);
}

void	find_biggest_line(t_ctrl *ctrl)
{
	size_t	i;
	size_t	len;

	len = 0;
	i = 0;
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		if (ctrl->map.columns < len)
		{
			ctrl->map.columns = len;
		}
		i++;
	}
	printf("DEBUG: number of columns: %zu\n", ctrl->map.columns);
}

void	explore_map(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*new_line;

	i = 0;
	j = 0;
	find_biggest_line(ctrl);
	while (i < ctrl->map.rows)
	{
		len = ft_strlen(ctrl->map.full_map[i]);
		printf("DEBUG: lenth of line %zu = %zu\n", i, len);
		if (len < ctrl->map.columns)
		{
			new_line = malloc(ctrl->map.columns + 1);
			ft_memcpy(new_line, ctrl->map.full_map[i], len);
			ft_memset(new_line + len, WALL, ctrl->map.columns - len);
			new_line[ctrl->map.columns] = '\0';
			free(ctrl->map.full_map[i]);
			ctrl->map.full_map[i] = new_line;
		}
		while (j < ctrl->map.columns)
		{
			if (ctrl->map.full_map[i][j] == ' ')
			{
				ctrl->map.full_map[i][j] = WALL;
			}
			j++;
		}
		j = 0;
		i++;
	}
}

void	check_valid_characters(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;
	char	c;
	//int		player_found;
	int count = 0;

	//player_found = 0;
	i = 0;
	j = 0;
	while (i < ctrl->map.rows)
	{
		while (j < ctrl->map.columns)
		{
			c = ctrl->map.full_map[i][j];
			if (!(c == WALL || c == EMPTY || c == NORTH || c == SOUTH
					|| c == EAST || c == WEST || c == ' '))
			{
				printf("Invalid input:\nMap contains invalid characters.\n");
				game_cleanup(ctrl);
				exit(1);
			}
			else if (c == NORTH || c == SOUTH || c == EAST || c == WEST)
			{
				ctrl->map.players_count++;
				//player_found = 1;
			}
			j++;
		}
		j = 0;
		i++;
	}
	if (ctrl->map.players_count == 0)
	{
		{
			printf("Invalid input:\nPlayer position not found.\n");
			game_cleanup(ctrl);
			exit(1);
		}
	}
	else
	{
		ctrl->map.players_positions = malloc(ctrl->map.players_count
				* sizeof(t_pos));
		i = 0;
		j = 0;
		while (i < ctrl->map.rows)
		{
			while (j < ctrl->map.columns)
			{
				c = ctrl->map.full_map[i][j];
				if (c == NORTH || c == SOUTH || c == EAST || c == WEST)
				{
                ctrl->map.players_positions[count].x = i;
                ctrl->map.players_positions[count].y = j;
				count++;
				}
				j++;
			}
			j = 0;
			i++;
		}
	}
}

void	check_map_closed(t_ctrl *ctrl)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < ctrl->map.columns)
	{
		if (ctrl->map.full_map[0][j] != WALL && ctrl->map.full_map[0][j] != ' ')
		{
			printf("Invalid input:\nMap must be closed by walls at the top.\n");
			game_cleanup(ctrl);
			exit(1);
		}
		if (ctrl->map.full_map[ctrl->map.rows - 1][j] != WALL
			&& ctrl->map.full_map[ctrl->map.rows - 1][j] != ' ')
		{
			printf("Invalid input:\nMap must be closed by walls at the bottom.\n");
			game_cleanup(ctrl);
			exit(1);
		}
		j++;
	}
	while (i < ctrl->map.rows)
	{
		if (ctrl->map.full_map[i][0] != WALL && ctrl->map.full_map[i][0] != ' ')
		{
			printf("Invalid input:\nMap must be closed by walls on the left.\n");
			game_cleanup(ctrl);
			exit(1);
		}
		if (ctrl->map.full_map[i][ctrl->map.columns - 1] != WALL
			&& ctrl->map.full_map[i][ctrl->map.columns - 1] != ' ')
		{
			printf("Invalid input:\nMap must be closed by walls on the right.\n");
			game_cleanup(ctrl);
			exit(1);
		}
		i++;
	}
}

char	**create_map_copy(t_ctrl *ctrl)
{
	char	**copy;
	size_t	i;

	i = 0;
	copy = malloc(ctrl->map.rows * sizeof(char *));
	while (i < ctrl->map.rows)
	{
		copy[i] = malloc(ctrl->map.columns + 1);
		if (!copy[i])
		{
			perror("Error allocating memory for map row copy");
			exit(1);
		}
		ft_strlcpy(copy[i], ctrl->map.full_map[i], ctrl->map.columns + 1);
		i++;
	}
	return (copy);
}

void	flood_fill(char **map, size_t x, size_t y, t_ctrl *ctrl,
		int *has_free_way)
{
	// 	// Check if out of map boundaries
	if (x >= ctrl->map.rows || y >= ctrl->map.columns)
		return ;
	// 	// If it's a wall or already visited cell, do nothing
	if (map[x][y] == WALL || map[x][y] == 'F')
		return ;
	// 	// Check for free paths
	if (map[x][y] == EMPTY)
		*has_free_way = 1;
	// 	// Mark the current cell as visited
	map[x][y] = 'F';
	// 		// Recursively check adjacent cells
	flood_fill(map, x + 1, y, ctrl, has_free_way); // Down
	flood_fill(map, x - 1, y, ctrl, has_free_way); // Up
	flood_fill(map, x, y + 1, ctrl, has_free_way); // Right
	flood_fill(map, x, y - 1, ctrl, has_free_way); // Left
}

// void	flood_fill(char **map, size_t x, size_t y, t_ctrl *ctrl,
// 		int *has_free_way)
// {
// 	// Check if out of map boundaries
// 	if (x >= ctrl->map.rows || y >= ctrl->map.columns || map[x][y] == '\0')
// 	{
// 		printf("Invalid input:\nMap must be closed by walls.\n");
// 		exit(1);
// 	}
// 	// If it's a wall or already visited cell, do nothing
// 	if (map[x][y] == WALL || map[x][y] == 'F')
// 		return ;
// 	// Check for free paths
// 	if (*has_free_way == 0)
// 	{
// 		if ((x > 0 && map[x - 1][y] != WALL && map[x - 1][y] != 'F')
// 			||                   // up
// 			(x < ctrl->map.rows - 1 && map[x + 1][y] != WALL && map[x
// 				+ 1][y] != 'F') ||  // down
// 			(y > 0 && map[x][y - 1] != WALL && map[x][y - 1] != 'F')
// 				||                   // left
// 			(y < ctrl->map.columns - 1 && map[x][y + 1] != WALL && map[x][y
// 				+ 1] != 'F')) // right
// 		{
// 			*has_free_way = 1; // At least one free path found
// 		}
// 	}
// 	// Mark the current cell as visited
// 	map[x][y] = 'F';
// 		// Recursively check adjacent cells
// 	if (x > 0)
// 		flood_fill(map, x - 1, y, ctrl, has_free_way); // Вверх
// 	if (x < ctrl->map.rows - 1)
// 		flood_fill(map, x + 1, y, ctrl, has_free_way); // Вниз
// 	if (y > 0)
// 		flood_fill(map, x, y - 1, ctrl, has_free_way); // Влево
// 	if (y < ctrl->map.columns - 1)
// 		flood_fill(map, x, y + 1, ctrl, has_free_way); // Вправо
// }

void	check_map_valid(t_ctrl *ctrl)
{
	char	**map_copy;
	size_t	x;
	size_t	y;
	int		has_free_way;
	size_t	count;

	count = 0;
	while (count < ctrl->map.players_count)
	{
		x = ctrl->map.players_positions[count].x;
		y = ctrl->map.players_positions[count].y;
		has_free_way = 0;
		map_copy = create_map_copy(ctrl);
		flood_fill(map_copy, x, y, ctrl, &has_free_way);
		free_map(map_copy, ctrl->map.rows);
		if (!has_free_way)
		{
			printf("Invalid input:\nPlayer has no space to move.\n");
			exit(1);
		}
		count++;
	}
	// printf("Map is valid and player has at least one free way to move.\n");
}

// void	check_map_valid(t_ctrl *ctrl)
// {
// 	char	**map_copy;
// 	size_t	x;
// 	size_t	y;

// 	x = ctrl->map.position.x;
// 	y = ctrl->map.position.y;
// 	map_copy = create_map_copy(ctrl);
// 	flood_fill(map_copy, x, y, ctrl);
// 	free_map(map_copy, ctrl->map.rows);
// 	printf("Map is valid and player has space to move.\n");
// }

void	parse_map(char *filename, t_ctrl *ctrl)
{
	char	*map_str;

	// char	*new_str;
	// int i = 0;
	map_str = read_map(filename, ctrl);
	ctrl->map.full_map = ft_split(map_str, '\n');
	if (!ctrl->map.full_map)
	{
		game_cleanup(ctrl);
		free(map_str);
		exit(1);
	}
	explore_map(ctrl);
	check_valid_characters(ctrl);
	check_map_closed(ctrl);
	check_map_valid(ctrl);
	// while (ctrl->map.full_map[i] != NULL)
	// {
	//     new_str = ft_strjoin(ctrl->map.full_map[i], "\n");
	//     ctrl->map.full_map[i] = new_str;
	//     free(new_str);
	//     i++;
	// }
}

void	init_game_window(t_game	*game)
{
	printf("insode init_game_window");
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		return ;
	}
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
	{
		free(game->mlx);
		return ;
	}
}

int	main(int argc, char **argv)
{
	t_ctrl	*ctrl;
	//t_game	*game;

	ctrl = malloc(sizeof(t_ctrl));
	if (!ctrl)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	ctrl->map.full_map = NULL;
	ctrl->map.rows = 0;
	ctrl->map.columns = 0;
	ctrl->map.players_count = 0;
	// ctrl->map.s_position = 0;
	// ctrl->map.e_position = 0;
	// ctrl->map.w_position = 0;
	check_args(argc, argv);
	parse_map(argv[1], ctrl);
	for (int i = 0; ctrl->map.full_map[i] != NULL; i++)
	{
		printf("%s\n", ctrl->map.full_map[i]);
	}
	printf("SUCCESS\n");
	// game = malloc(sizeof(t_game));
	// if (!game)
	// 	return (EXIT_FAILURE);
	// game->mlx = NULL;
	// game->win = NULL;
	// init_game_window(game);
	// mlx_loop(game->mlx);
	return (0);
}
