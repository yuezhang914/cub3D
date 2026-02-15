/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitlines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:52 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 17:49:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static int	sfill_array(t_game *game, char *pos, int line_count, char **result)
{
	size_t	len;
	int		i;

	i = 0;
	while (i < line_count - 1)
	{
		len = ft_strchr(pos, '\n') - pos;
		result[i] = track_malloc(game, (len + 1) * sizeof(char));
		ft_memcpy(result[i], pos, len);
		result[i][len] = '\0';
		pos = ft_strchr(pos, '\n') + 1;
		i++;
	}
	len = ft_strchr(pos, '\0') - pos;
	result[i] = track_malloc(game, (len + 1) * sizeof(char));
	ft_memcpy(result[i], pos, len);
	result[i][len] = '\0';
	result[line_count] = NULL;
	return (0);
}

char	**splitlines(t_game *game, char *text)
{
	int		line_count;
	char	**result;

	line_count = char_count(text, '\n') + 1;
	if (line_count == 0)
		return (NULL);
	result = track_malloc(game, (line_count + 1) * sizeof(void *));
	if (result == NULL)
		return (NULL);
	if (sfill_array(game, text, line_count, result) == 1)
		return (NULL);
	return (result);
}
