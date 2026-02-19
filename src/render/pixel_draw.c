/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 13:45:23 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/*
** 函数：put_pixel
** 作用：往画布写一个像素（带边界检查）
** 参数：
**   x/y：屏幕坐标
**   color：颜色值（int）
**   game：包含 data/size_line/bpp 的图像信息
** 逻辑：
**   1) 越界直接 return
**   2) 计算目标地址 dst
**   3) 写入 color
** 在哪调用：clear_image(), render_column(), 小地图/精灵等绘制模块
*/
void	put_pixel(int x, int y, int color, t_game *game)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->data + (y * game->size_line + x * (game->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

/*
** 函数：draw_wall
** 作用：绘制一列墙体贴图（从 v.start 到 v.end），做简单纹理映射
** 参数：
**   v：该列渲染参数（x、start/end、tex、tex_x 等）
**   game：游戏结构体（用于 put_pixel）
**   step：纹理 y 每画 1 像素要增加多少
**   tex_pos：纹理 y 的起始位置
** 逻辑：
**   1) y 从 start 到 end
**   2) 根据 tex_pos 取 tex_y 和 color
**   3) put_pixel 画到屏幕
** 在哪调用：render_column() 的 draw_wall_part()
*/
void	draw_wall(t_render_vars v, t_game *game, float step, float tex_pos)
{
	int	y;
	int	tex_y;
	int	color;

	y = v.start;
	if (y < 0)
		y = 0;
	while (y <= v.end && y < HEIGHT)
	{
		tex_y = (int)tex_pos % v.tex->height;
		color = *(int *)(v.tex->data
				+ (tex_y * v.tex->size_line + v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
}
