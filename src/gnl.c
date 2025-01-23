/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:27:29 by anilchen          #+#    #+#             */
/*   Updated: 2025/01/23 14:43:18 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1000000
#endif

#include "../includes/cub3d.h"

static char	*gnl_joining_to_save(char *save, char *buffer)
{
	char	*new_save;

	if (!save)
		save = ft_strdup("");
	new_save = ft_strjoin(save, buffer);
	if (!new_save)
		return (NULL);
	free(save);
	save = NULL;
	return (new_save);
}

static char	*gnl_reading(int fd, char *buffer, char *save)
{
	int	bytes_read;

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(save);
			return (NULL);
		}
		if (bytes_read == 0)
		{
			free(buffer);
			break ;
		}
		buffer[bytes_read] = '\0';
		save = gnl_joining_to_save(save, buffer);
		if (ft_strchr(save, '\n'))
		{
			free(buffer);
			break ;
		}
	}
	return (save);
}

char	*gnl_extracting(char *data)
{
	int		count;
	char	*line;
	int		i;

	i = 0;
	count = 0;
	while (data[count] && data[count] != '\n')
		count++;
	if (data[count] == '\0')
	{
		line = ft_strdup(data);
		return (line);
	}
	line = malloc((count + 2) * sizeof(char));
	if (!line)
		return (NULL);
	while (i < count + 1)
	{
		line[i] = data[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*gnl_trimming(char *data)
{
	int		count;
	char	*save;

	count = 0;
	while (data[count] && data[count] != '\n')
		count++;
	if (data[count] == '\0')
		return (0);
	save = ft_strdup(data + count + 1);
	if (!save || *save == '\0')
	{
		free(save);
		return (NULL);
	}
	return (save);
}

char	*gnl(int fd, t_ctrl *ctrl)
{
	char	*buffer;
	char	*data;

	buffer = (char *)malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!buffer)
		return (NULL);
	if (BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buffer);
		free(ctrl->gnl.save);
		ctrl->gnl.save = NULL;
		return (NULL);
	}
	data = gnl_reading(fd, buffer, ctrl->gnl.save);
	if (!data)
	{
		return (NULL);
	}
	ctrl->gnl.line = gnl_extracting(data);
	ctrl->gnl.save = gnl_trimming(data);
	free(data);
	return (ctrl->gnl.line);
}
