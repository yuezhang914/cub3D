/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framebuffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:04:04 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 01:12:55 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

/*
** 函数：put_pixel
** 作用：把一个像素写进屏幕帧缓冲（game->data）。
** 参数：
**   x,y   ：屏幕坐标
**   color ：颜色（0 也允许写入，用于清屏/透明）
**   game  ：提供 data / bpp / size_line
** 用在哪：
**   clear_image、画墙、画线、画方块等所有绘图函数都会用它。
*/
void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
		return ;
	index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
	if (game->bpp == 32)
		game->data[index + 3] = 0;
}

/*
** 函数：clear_image
** 作用：清空整个屏幕（把所有像素写成 0）。
** 参数：
**   game：提供宽高与 put_pixel
** 用在哪：
**   每一帧开始时清屏，防止上一帧残影。
*/
void	clear_image(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}
