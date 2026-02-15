/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:02:20 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 17:10:35 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static size_t	s_ft_strlen_safe(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void	s_write_str(int fd, const char *s)
{
	if (!s)
		return ;
	write(fd, s, s_ft_strlen_safe(s));
}

void	print_error(const char *func, char *message)
{
	s_write_str(2, "Error\n");
	if (func && *func)
		s_write_str(2, func);
	else
		s_write_str(2, "unknown");
	s_write_str(2, ": ");
	if (message && *message)
		s_write_str(2, message);
	else
		s_write_str(2, "unknown error");
	s_write_str(2, "\n");
}
