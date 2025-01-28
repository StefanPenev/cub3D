/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:09:47 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/28 15:55:47 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
	{
		return (0);
	}
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return (0);
}

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

void	assign_path(t_texture *tex, char *path, char *line, t_ctrl *ctrl)
{
	int		i;
	char	*base_path;
	char	*frame_num;
	size_t	len;

	frame_num = NULL;
	if (tex->path != NULL)
	{
		free(line);
		free_and_exit("Wrong input:\nTexture is duplicated\n", path, NULL,
			ctrl);
	}
	//tex->path = ft_strdup(path);
	// if (!tex->path)
	// 	free_and_exit("Memory allocation failed\n", path, NULL, ctrl);
	len = ft_strlen(path);
	if (len < 5 || ft_strcmp(path + len - 5, "0.xpm") != 0)
		free_and_exit("Invalid texture path\n", path, NULL, ctrl);
	base_path = ft_strndup(path, len - 5);
	if (!base_path)
		free_and_exit("base_path: memory allocation failed\n", path, NULL, ctrl);
	i = 0;
	while (i < MAX_FRAMES)
	{
		if (i == 0)
		{
			tex->paths[i] = ft_strdup(path);
			free(path);
			path = NULL;
		}
		else
		{
			frame_num = ft_itoa(i);
			if (!frame_num)
			{
				free(base_path);
				free_and_exit("frame_num: memory allocation failed\n", NULL, NULL, ctrl);
			}
			tex->paths[i] = ft_strjoin_three(base_path, frame_num, ".xpm");
			free(frame_num);
		}
		if (!tex->paths[i])
		{
			free(frame_num);
			free(base_path);
			free_and_exit("tex->frames[i]: Memory allocation failed\n", NULL, NULL, ctrl);
		}
		i++;
	}
	free(base_path);
	for (int j = 0; j < MAX_FRAMES; j++)
		printf("DEBUG: %s\n", tex->paths[j]);
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
