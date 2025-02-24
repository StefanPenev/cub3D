/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:09:47 by anilchen          #+#    #+#             */
/*   Updated: 2025/02/24 13:08:02 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

// Creates a duplicate of the first 'n' characters from a given string.
// - Allocates memory for the new string, including the null-terminator.
// - Copies up to 'n' characters from the source string into the new one.
// - Returns a pointer to the newly allocated string or NULL on failure.

char	*ft_strndup(const char *s, size_t n)
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

// Concatenates three strings into a single newly allocated string.
// - Calculates the total length of all three strings.
// - Allocates memory for the combined string.
// - Copies each string into the new memory block using ft_memcpy.
// - Returns a pointer to the combined string or NULL if all inputs are NULL.

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	int		len_s1;
	int		len_s2;
	int		len_s3;
	char	*res;

	len_s1 = 0;
	len_s2 = 0;
	len_s3 = 0;
	if (!s1 && !s2 && !s3)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len_s3 = ft_strlen(s3);
	res = (char *)malloc(len_s1 + len_s2 + len_s3 + 1);
	ft_memcpy(res, s1, len_s1);
	ft_memcpy(res + len_s1, s2, len_s2);
	ft_memcpy(res + len_s1 + len_s2, s3, len_s3);
	res[len_s1 + len_s2 + len_s3] = '\0';
	return (res);
}

// Checks if a given line defines a texture path for the map.
// - Returns 1 if the line starts with "NO ", "SO ", "WE ", or "EA "
//(texture prefixes).
// - Returns 0 otherwise.

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

// Assigns a texture path to the corresponding texture object.
// - Checks for duplicate texture definitions and exits on error.
// - Verifies that the texture path ends with "0.xpm".
// - Extracts the base path and generates frame paths for animated textures.
// - Handles memory allocation failures gracefully.

void	assign_path(t_texture *tex, char *path, char *line, t_ctrl *ctrl)
{
	char	*base_path;
	size_t	len;

	if (tex->paths[0] != NULL)
	{
		free(line);
		free_and_exit("Wrong input:\nTexture is duplicated\n", path, NULL,
			ctrl);
	}
	len = ft_strlen(path);
	if (len < 5 || ft_strcmp(path + len - 5, "0.xpm") != 0)
	{
		free(line);
		free_and_exit("Invalid texture path\n", path, NULL, ctrl);
	}
	base_path = ft_strndup(path, len - 5);
	if (!base_path)
		free_and_exit("base_path: memory allocation failed\n", path, NULL,
			ctrl);
	tex->paths[0] = ft_strdup(path);
	free(path);
	path = NULL;
	create_frame_paths(tex, base_path, ctrl);
	free(base_path);
}

// Processes and assigns texture paths based on map configuration lines.
// - Extracts and cleans the texture path from the input line.
// - Validates that the texture file has a ".xpm" extension.
// - Calls assign_path() to link the path to the correct texture slot
// - Exits with an error if the path format or extension is invalid.

void	keep_textures_path(char *line, t_ctrl *ctrl)
{
	char	*trimmed_line;
	char	*path;
	char	*clean_path;
	int		len;

	trimmed_line = trim_trailing_whitespace(line);
	path = ft_strdup(trimmed_line + 3);
	if (!path)
		clean_exit("path: Memory allocation failed\n", ctrl);
	clean_path = remove_inner_spaces(path, ctrl);
	len = ft_strlen(clean_path);
	if (len < 4 || ft_strncmp(clean_path + len - 4, ".xpm", 4) != 0)
	{
		free(trimmed_line);
		free_and_exit("Wrong input:\nTexture should have .xpm extension.\n ",
			clean_path, NULL, ctrl);
	}
	if (ft_strncmp("NO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->north_texture, clean_path, line, ctrl);
	else if (ft_strncmp("SO ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->south_texture, clean_path, line, ctrl);
	else if (ft_strncmp("WE ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->west_texture, clean_path, line, ctrl);
	else if (ft_strncmp("EA ", trimmed_line, 3) == 0)
		assign_path(&ctrl->game->east_texture, clean_path, line, ctrl);
}
