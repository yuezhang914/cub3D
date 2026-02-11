/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:08:07 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 17:36:30 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static const char	*index_substr(const char *str, int index, char dlm)
{
	int	char_sctn;

	char_sctn = 0;
	while (*str && index > 0)
	{
		if (char_sctn == 1 && *str == dlm)
			index--;
		if (*str != dlm)
			char_sctn = 1;
		if (*str == dlm)
			char_sctn = 0;
		str++;
	}
	while (*str && *str == dlm)
		str++;
	if (!*str)
		return (NULL);
	return (str);
}

static int	substr_len(char const *str, char dlm, int i)
{
	int	cnt;

	str = index_substr(str, i, dlm);
	cnt = 0;
	while (*str != dlm && *str)
	{
		str++;
		cnt++;
	}
	return (cnt);
}

static void	substr_cpy(char const *str, char dlm, int i, char *sub)
{
	str = index_substr(str, i, dlm);
	while (*str != dlm && *str)
	{
		*sub++ = *str++;
	}
	*sub = '\0';
}

char	**ft_split(t_game *game, char const *s, char c)
{
	int		sub_cnt;
	char	**ptr;
	int		i;

	sub_cnt = 0;
	while (index_substr(s, sub_cnt, c))
		sub_cnt++;
	ptr = track_malloc(game, (sub_cnt + 1) * sizeof(void *));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < sub_cnt)
	{
		ptr[i] = track_malloc(game, (substr_len(s, c, i) + 1) * sizeof(char));
		if (ptr[i] == NULL)
			return (NULL);
		substr_cpy(s, c, i, ptr[i]);
		i++;
	}
	ptr[i] = NULL;
	return (ptr);
}
