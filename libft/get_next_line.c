/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:45:48 by anilchen          #+#    #+#             */
/*   Updated: 2024/08/19 17:30:59 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1000000
#endif

#include "libft.h"

static char	*joining_to_save(char *save, const char *buffer)
{
	char	*new_save;

	if (!save)
		save = ft_strdup("");
	new_save = ft_strjoin(save, buffer);
	if (!new_save)
		return (NULL);
	free(save);
	return (new_save);
}

static char	*reading(int fd, char *buffer, char *save)
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
		save = joining_to_save(save, buffer);
		if (ft_strchr(save, '\n'))
		{
			free(buffer);
			break ;
		}
	}
	return (save);
}

char	*extracting(char *data)
{
	int		count;
	char	*line;
	int		i;

	count = 0;
	i = 0;
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

static char	*trimming(char *data)
{
	int		count;
	char	*save;

	count = 0;
	while (data[count] && data[count] != '\n')
		count++;
	if (data[count] == '\0')
		return (NULL);
	save = ft_strdup(data + count + 1);
	if (!save || *save == '\0')
	{
		free(save);
		return (NULL);
	}
	return (save);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*data;
	char		*line;
	static char	*save[1024];

	buffer = (char *)malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!buffer)
		return (NULL);
	if (fd >= 1024 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buffer);
		free(save[fd]);
		save[fd] = NULL;
		return (NULL);
	}
	data = reading(fd, buffer, save[fd]);
	if (!data)
	{
		return (NULL);
	}
	line = extracting(data);
	save[fd] = trimming(data);
	free(data);
	return (line);
}

// int	main(void)
// {
// 	int		fd;
// 	char	*next_line;
// 	int		count;

// 	count = 0;
// 	// fd = 42;
// 	fd = open("example.txt", O_RDONLY);
// 	while (1)
// 	{
// 		next_line = get_next_line(fd);
// 		if (next_line == NULL)
// 		{
// 			break ;
// 		}
// 		count++;
// 		printf("[%d]:%s", count, next_line);
// 		free(next_line);
// 		next_line = NULL;
// 	}
// 	free(next_line);
// 	next_line = NULL;
// 	// char *buffer = get_next_line(fd);
// 	// printf("%s", buffer);
// 	close(fd);
// 	return (0);
// }
