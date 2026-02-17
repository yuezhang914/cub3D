/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:05:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 01:13:45 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

/*
** 函数：draw_image
** 作用：把整张纹理按原尺寸画到屏幕（不缩放）。
** 参数：
**   game：用于 put_pixel
**   tex ：要画的纹理
**   top_left：屏幕左上角位置
** 用在哪：
**   小地图图标、UI 图标、调试贴图等。
*/
void	draw_image(t_game *game, t_tex *tex, t_int_xy top_left)
{
	int		x;
	int		y;
	t_coords	uv;
	int		color;

	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			uv.x = (float)x / (float)tex->width;
			uv.y = (float)y / (float)tex->height;
			color = tex_sample(tex, uv);
			put_pixel(top_left.x + x, top_left.y + y, color, game);
			x++;
		}
		y++;
	}
}

/*
** 函数：draw_from_tex_to_area
** 作用：把纹理缩放后画进一个矩形区域（常用于 logo、UI、缩放贴图）。
** 参数：
**   game：用于 put_pixel
**   tex ：源纹理
**   area：目标区域（左上角 + 宽高）
** 用在哪：
**   draw_win_logo_if_finished() 把 win_logo 缩放/居中贴到屏幕。
*/
void	draw_from_tex_to_area(t_game *game, t_tex *tex, t_rect area)
{
	int		x;
	int		y;
	t_coords	uv;
	int		color;

	y = 0;
	while (y < area.height)
	{
		x = 0;
		while (x < area.width)
		{
			uv.x = (float)x / (float)area.width;
			uv.y = (float)y / (float)area.height;
			color = tex_sample(tex, uv);
			put_pixel(area.top_left.x + x, area.top_left.y + y, color, game);
			x++;
		}
		y++;
	}
}
