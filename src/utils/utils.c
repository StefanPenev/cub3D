/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 09:59:39 by stefan            #+#    #+#             */
/*   Updated: 2025/01/24 20:25:39 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
