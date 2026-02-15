/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:56 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:01:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 向图像缓冲区写入颜色
 * index 计算公式：(y * 宽度 + x) * 每个像素占用的字节数
 */
void put_pixel(int x, int y, int color, t_game *game)
{
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return;
	int index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;			  
	game->data[index + 1] = (color >> 8) & 0xFF;  
	game->data[index + 2] = (color >> 16) & 0xFF; 
	game->data[index + 3] = 0xFF;		
}

/**
 * 渲染屏幕的一列像素：天花板 -> 墙壁 -> 地板
 */
/**
 * 渲染屏幕的一列像素 (符合 Norm 规范)
 * 作用：利用封装后的 v 结构体获取渲染区间和纹理信息，
 * 依次在屏幕第 x 列绘制天花板、经过缩放处理的墙体纹理以及地板。
 * 参数：v - 包含所有列渲染所需变量的结构体；game - 游戏主结构体。
 */
void render_column(t_render_vars v, t_game *game)
{
	int y;
	float step;
	float tex_pos;
	int color;

	y = -1;
	while (++y < (v.start < 0 ? 0 : v.start))
		put_pixel(v.x, y, game->ceiling_color, game);
	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = (v.start < 0 ? (0 - v.start) : 0) * step;
	y = (v.start < 0) ? 0 : v.start;
	while (y <= (v.end >= HEIGHT ? HEIGHT - 1 : v.end))
	{
		color = *(int *)(v.tex->data + ((int)tex_pos % v.tex->height *
											v.tex->size_line +
										v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
	y--;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}
