/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitives.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:05:51 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 01:14:11 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

/*
** 函数：draw_line
** 作用：画一条线段（用“走很多小步”的方法从 start 走到 end）。
** 参数：
**   game：用于 put_pixel
**   line：包含颜色 + 起点 + 终点
** 用在哪：
**   调试射线、小地图画线、画 UI 边框等。
*/
void	draw_line(t_game *game, t_line line)
{
	t_coords	step;
	int		steps;
	int		i;

	step.x = line.end.x - line.start.x;
	step.y = line.end.y - line.start.y;
	steps = (int)fmaxf(fabsf(step.x), fabsf(step.y));
	if (steps <= 0)
		return ;
	step.x /= steps;
	step.y /= steps;
	i = 0;
	while (i <= steps)
	{
		put_pixel(line.start.x + i * step.x, line.start.y + i * step.y,
			line.color, game);
		i++;
	}
}

/*
** 函数：draw_square
** 作用：只画正方形边框（不填充）。
** 参数：
**   game：用于 put_pixel
**   sq  ：颜色、边长、左上角
** 用在哪：
**   小地图格子边框、调试框等。
*/
void	draw_square(t_game *game, t_square sq)
{
	int	i;

	i = 0;
	while (i <= sq.size)
	{
		put_pixel(sq.top_left.x + i, sq.top_left.y, sq.color, game);
		put_pixel(sq.top_left.x + i, sq.top_left.y + sq.size, sq.color, game);
		put_pixel(sq.top_left.x, sq.top_left.y + i, sq.color, game);
		put_pixel(sq.top_left.x + sq.size, sq.top_left.y + i, sq.color, game);
		i++;
	}
}

/*
** 函数：draw_full_square
** 作用：画一个填充的正方形。
** 参数：
**   game：用于 put_pixel
**   sq  ：颜色、边长、左上角
** 用在哪：
**   小地图画墙格/玩家点等。
*/
void	draw_full_square(t_game *game, t_square sq)
{
	int	i;
	int	j;

	i = 0;
	while (i < sq.size)
	{
		j = 0;
		while (j < sq.size)
		{
			put_pixel(sq.top_left.x + i, sq.top_left.y + j, sq.color, game);
			j++;
		}
		i++;
	}
}

/*
** 函数：draw_circle_full
** 作用：画一个填充圆（修复了原版起点/边界错误）。
** 参数：
**   game：用于 put_pixel
**   circle：中心、半径、颜色
** 用在哪：
**   小地图玩家圆点、圆形标记等。
*/
void	draw_circle_full(t_game *game, t_circle circle)
{
	int	x;
	int	y;
	int	x0;
	int	y0;

	x0 = (int)(circle.center.x - circle.radius);
	y0 = (int)(circle.center.y - circle.radius);
	x = x0;
	while (x <= (int)(circle.center.x + circle.radius))
	{
		y = y0;
		while (y <= (int)(circle.center.y + circle.radius))
		{
			if (distance(circle.center, (t_coords){x, y}) <= circle.radius)
				put_pixel(x, y, circle.color, game);
			y++;
		}
		x++;
	}
}
