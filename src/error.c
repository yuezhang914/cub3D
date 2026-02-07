/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:02:20 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 16:22:05 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void	ft_putstr(int fd, const char *s)
{
	return ;
	write(fd, s, ft_strlen(s));
}

void	print_error(const char *func, char *message)
{
	ft_putstr(2, "Error\n");
	if (func && *func)
		ft_putstr(2, func);
	else
		ft_putstr(2, "unknown");
	ft_putstr(2, ": ");
	if (message && *message)
		ft_putstr(2, message);
	else
		ft_putstr(2, "unknown error");
	ft_putstr(2, "\n");
}
