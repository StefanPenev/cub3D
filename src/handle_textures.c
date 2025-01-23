/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:09:47 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/23 15:21:02 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_texture_definition(char *line_tmp)
{
	if (ft_strncmp("NO ", line_tmp, 3) == 0 || ft_strncmp("SO ", line_tmp,
			3) == 0 || ft_strncmp("WE ", line_tmp, 3) == 0 || ft_strncmp("EA ",
			line_tmp, 3) == 0)
	{
		return (1);
	}
	return (0);
}

void	assign_path(char **str, char *path, char *line, t_ctrl *ctrl)
{
	if (*str != NULL)
	{
		free(line);
		free_and_exit("Wrong input:\nTexture is duplicated\n", path, NULL,
			ctrl);
	}
	*str = path;
}

void	keep_textures_path(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	*path;
	char	*clean_path;
	int		len;

	trimmed_line = trim_trailing_whitespace(line);
	path = ft_strdup(trimmed_line + 3);
	if (!path)
		clean_exit("Memory allocation failed\n", ctrl);
	clean_path = remove_inner_spaces(path, ctrl);
	len = ft_strlen(clean_path);
	if (len < 4 || ft_strncmp(clean_path + len - 4, ".xpm", 4) != 0)
	{
		free(trimmed_line);
		free_and_exit("Wrong input:\nTexture should have .xpm extension.\n ",
			clean_path, NULL, ctrl);
	}
	if (ft_strncmp("NO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->north_texture.path, clean_path, line, ctrl);
	else if (ft_strncmp("SO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->south_texture.path, clean_path, line, ctrl);
	else if (ft_strncmp("WE ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->west_texture.path, clean_path, line, ctrl);
	else if (ft_strncmp("EA ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->east_texture.path, clean_path, line, ctrl);
}
