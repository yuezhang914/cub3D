/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:12:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:37:39 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

char	*ft_readfile(t_game *game, int fd)
{
	char	*str;
	char	buf[BUFFER_SIZE];
	int		read_rt;

	str = track_malloc(game, sizeof(char));
	if (str == NULL)
		return (NULL);
	*str = '\0';
	while (1)
	{
		ft_bzero(buf, BUFFER_SIZE);
		read_rt = read(fd, buf, BUFFER_SIZE - 1);
		if (read_rt == -1)
			return (NULL);
		if (read_rt == 0)
			break ;
		str = ft_strjoin(game, str, buf);
		if (str == NULL)
			return (NULL);
	}
	return (str);
}
