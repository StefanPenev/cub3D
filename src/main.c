/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:36 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/13 16:30:31 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_map(char **map)
{
	int	i;

	i = 0;
	if (!map)
		return ;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	game_cleanup(t_ctrl *ctrl)
{
	if (ctrl->map.full_map)
	{
		free_map(ctrl->map.full_map);
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
			ft_memset(new_line + len, WALL, ctrl->map.columns + len);
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
	// while (ctrl->map.full_map[i] != NULL)
	// {
	//     new_str = ft_strjoin(ctrl->map.full_map[i], "\n");
	//     ctrl->map.full_map[i] = new_str;
	//     free(new_str);
	//     i++;
	// }
}

int	main(int argc, char **argv)
{
	t_ctrl	*ctrl;

	ctrl = malloc(sizeof(t_ctrl));
	if (!ctrl)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	ctrl->map.full_map = NULL;
	ctrl->map.rows = 0;
	ctrl->map.columns = 0;
	check_args(argc, argv);
	parse_map(argv[1], ctrl);
	for (int i = 0; ctrl->map.full_map[i] != NULL; i++)
	{
		printf("%s\n", ctrl->map.full_map[i]);
	}
	printf("SUCCESS\n");
	return (0);
}
