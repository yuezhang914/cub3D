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

#include "cub3d.h"

/**
 * @brief 将单个像素写入图像缓冲区。
 * * @param x, y  目标像素在屏幕上的坐标。
 * @param color 16进制颜色值 (0xRRGGBB)。
 * @param game  游戏主结构体，包含图像 data 指针和 line_size 等信息。
 * * 逻辑：计算一维数组索引 index = (y * 线宽) + (x * 像素字节数)。
 * 这里的写入顺序符合小端序 (BGRA)。
 */
void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
	game->data[index + 3] = 0;
}

/**
 * @brief 绘制垂直线中的“墙体”部分，并应用纹理映射。
 * * @param v       存储渲染所需的变量（如 start, end, tex_x, 纹理指针等）。
 * @param game    游戏主结构体。
 * @param step    纹理坐标的步长（纹理高度 / 屏幕墙高）。
 * @param tex_pos 纹理在 Y 轴上的初始读取位置。
 */
static void	draw_wall(t_render_vars v, t_game *game, float step, float tex_pos)
{
	int	y;
	int	color;
	int	tex_y;

	y = v.start;
	if (y < 0)
		y = 0;
	while (y <= v.end && y < HEIGHT)
	{
		tex_y = (int)tex_pos % v.tex->height;
		color = *(int *)(v.tex->data + (tex_y * v.tex->size_line
					+ v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
}

/**
 * @brief 渲染屏幕的第 i 列（一整个垂直线）。
 * * @param game     游戏主结构体。
 * @param r_dir_x  当前光线的 X 方向向量。
 * @param r_dir_y  当前光线的 Y 方向向量。
 * @param i        当前处理的屏幕横坐标 (0 到 WIDTH-1)。
 */
void	render_column(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	int				y;
	float			step;
	float			tex_pos;
	t_render_vars	v;

	v = get_render_vars(game, r_dir_x, r_dir_y, i);
	y = -1;
	while (++y < v.start && y < HEIGHT)
		put_pixel(v.x, y, game->ceiling_color, game);
	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = 0;
	if (v.start < 0)
		tex_pos = (float)-v.start * step;
	draw_wall(v, game, step, tex_pos);
	y = v.end;
	if (y < 0)
		y = -1;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}