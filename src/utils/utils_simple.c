/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:35 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:36:33 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	char_count(char *str, char c)
{
	int	count;

	count = 0;
	if (!str)
		return (-1);
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

bool	is_dig(char c)
{
	if ((c >= '0' && c <= '9'))
		return (true);
	else
		return (false);
}

char	*ft_n_strcmp(char *str1, int n, ...)
{
	va_list	strs;
	char	*str;

	va_start(strs, n);
	while (n)
	{
		str = va_arg(strs, char *);
		if (ft_strcmp(str, str1) == 0)
		{
			va_end(strs);
			return (str);
		}
		n--;
	}
	va_end(strs);
	return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest2;
	unsigned char	*src2;

	dest2 = (unsigned char *)dest;
	src2 = (unsigned char *)src;
	if (!dest || !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest2[i] = src2[i];
		i++;
	}
	return (dest);
}
