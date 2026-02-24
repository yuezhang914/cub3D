/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 13:45:23 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

void	draw_wall(t_render_vars v, t_game *game, float step, float tex_pos)
{
	int	y;
	int	tex_y;
	int	color;

	/* 1. 确定绘制起点 */
	/* v.start 是计算出的墙体顶端像素行。如果超出屏幕上方(负数)，则从0开始 */
	y = v.start;
	if (y < 0)
		y = 0;

	/* 2. 垂直扫描：从墙顶画到墙底 */
	/* 循环条件确保不会画出屏幕底端 (HEIGHT) */
	while (y <= v.end && y < HEIGHT)
	{
		/* 3. 计算当前的纹理 Y 坐标 */
		/* tex_pos 记录了当前像素对应的纹理精确浮点位置 */
		/* 取模运算是为了防止越界，并将其转为整数索引 tex_y */
		tex_y = (int)tex_pos % v.tex->height;

		/* 4. 从纹理数据中提取颜色 */
		/* 使用指针偏移计算像素地址：基地址 + (行偏移 + 列偏移) */
		/* v.tex->size_line 是一行纹理占用的字节数 */
		color = *(int *)(v.tex->data
				+ (tex_y * v.tex->size_line + v.tex_x * (v.tex->bpp / 8)));

		/* 5. 将颜色写入图像缓冲区 */
		/* 在屏幕坐标 (v.x, y) 处放置计算出的颜色 */
		put_pixel(v.x, y, color, game);

		/* 6. 步进纹理位置 */
		/* step 代表屏幕上移动 1 像素对应纹理上移动多少距离 */
		/* 如果墙很近（画得高），step 就会很小；墙很远则 step 很大 */
		tex_pos += step;
		y++;
	}
}
