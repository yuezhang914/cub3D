/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:02:20 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 22:00:50 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


static void	put_str(int fd, const char *s)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	print_error(const char *func, const char *message)
{
	/* subject 格式：第一行必须是 "Error" */
	put_str(2, "Error\n");
	/* 第二行：<func>: <message> */
	if (func && *func)
		put_str(2, func);
	else
		put_str(2, "unknown");
	put_str(2, ": ");
	if (message && *message)
		put_str(2, message);
	else
		put_str(2, "unknown error");
	put_str(2, "\n");
}
