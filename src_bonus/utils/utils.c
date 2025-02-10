/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 09:59:39 by stefan            #+#    #+#             */
/*   Updated: 2025/02/10 14:54:15 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub3d.h"

int	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*trim_trailing_whitespace(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	while (len > 0 && (ft_isspace((unsigned char)str[len - 1])))
	{
		str[len - 1] = '\0';
		len--;
	}
	return (str);
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

void	validate_frame_path(t_texture *tex, int i)
{
	int	fd;

	fd = open(tex->paths[i], O_RDONLY);
	if (fd == -1)
	{
		free(tex->paths[i]);
		tex->paths[i] = ft_strdup(tex->paths[0]);
	}
	close(fd);
}

void	create_frame_paths(t_texture *tex, char *base_path, t_ctrl *ctrl)
{
	int		i;
	char	*frame_num;

	frame_num = NULL;
	i = 1;
	while (i < MAX_FRAMES)
	{
		frame_num = ft_itoa(i);
		if (!frame_num)
			free_and_exit("frame_num: memory allocation failed\n", base_path,
				NULL, ctrl);
		tex->paths[i] = ft_strjoin_three(base_path, frame_num, ".xpm");
		free(frame_num);
		validate_frame_path(tex, i);
		if (!tex->paths[i])
			free_and_exit("tex->frames[i]: Memory allocation failed\n",
				base_path, NULL, ctrl);
		i++;
	}
}
