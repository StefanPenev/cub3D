/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:06:05 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/19 15:59:50 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

// Checks if a given line defines a floor (F) or ceiling (C) color.
// - Returns 1 if the line starts with 'F ' or 'C '.
// - Returns 0 otherwise.

int	is_color_definition(char *line_tmp)
{
	if (ft_strncmp("F ", line_tmp, 2) == 0 || ft_strncmp("C ", line_tmp,
			2) == 0)
	{
		return (1);
	}
	return (0);
}

// Validates that the RGB color line contains only digits, commas, and spaces.
// - Iterates through each character in the cleaned line.
// - Exits with an error if any invalid character is found.

void	validate_rgb_line(char *clean_line_tmp, char *line, t_ctrl *ctrl)
{
	int	i;

	i = 0;
	while (clean_line_tmp[i] != '\0')
	{
		if (!ft_isdigit(clean_line_tmp[i]) && clean_line_tmp[i] != ','
			&& clean_line_tmp[i] != ' ')
		{
			free(clean_line_tmp);
			free_and_exit("Wrong input:\nInvalid color definition\n", line,
				NULL, ctrl);
		}
		i++;
	}
}

// Splits the RGB color definition line by commas and validates its structure.
// - Removes extra spaces within each RGB component.
// - Ensures that exactly three components (R, G, B) are present.
// - Exits with an error if validation fails or memory allocation issues occur.

char	**split_and_check(char *line, t_ctrl *ctrl)
{
	int		i;
	char	*clean_line_tmp;
	char	**tmp_arr;
	char	*new_line;

	i = 0;
	clean_line_tmp = ft_strdup(line + 2);
	if (!clean_line_tmp)
		free_and_exit("Memory allocation failed\n", line, NULL, ctrl);
	validate_rgb_line(clean_line_tmp, line, ctrl);
	tmp_arr = ft_split(clean_line_tmp, ',');
	free(clean_line_tmp);
	if (!tmp_arr)
		free_and_exit("Memory allocation failed\n", line, NULL, ctrl);
	while (tmp_arr[i] != NULL)
	{
		new_line = remove_inner_spaces(tmp_arr[i], ctrl);
		tmp_arr[i] = new_line;
		i++;
	}
	if (i != 3)
		free_and_exit("Wrong input:\nInvalid color definition\n", line, tmp_arr,
			ctrl);
	return (tmp_arr);
}

// Parses and assigns floor (F) or ceiling (C) colors
// from a map configuration line.
// - Extracts and validates RGB values from the line.
// - Converts RGB values to a single integer using bit-shifting.
// - Updates the corresponding color field (floor or ceiling)
// in the game structure.
// - Ensures RGB values are within the valid range (0-255).

void	define_fc_colors(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	**tmp_arr;
	int		r;
	int		g;
	int		b;

	r = 0;
	g = 0;
	b = 0;
	trimmed_line = trim_trailing_whitespace(line);
	tmp_arr = split_and_check(trimmed_line, ctrl);
	r = ft_atoi(tmp_arr[0]);
	g = ft_atoi(tmp_arr[1]);
	b = ft_atoi(tmp_arr[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		free_and_exit("Wrong input:\nRGB values must be between 0 and 255\n",
			trimmed_line, tmp_arr, ctrl);
	if (ft_strncmp("F ", trimmed_line, 2) == 0)
		ctrl->game->floor_color = (r << 16) | (g << 8) | b;
	else if (ft_strncmp("C ", trimmed_line, 2) == 0)
		ctrl->game->ceiling_color = (r << 16) | (g << 8) | b;
	else
		free_and_exit("Wrong input:\nUnknown color definition\n", trimmed_line,
			tmp_arr, ctrl);
	free_map(tmp_arr, 3);
}
