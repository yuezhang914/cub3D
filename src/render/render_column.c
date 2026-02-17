/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:56 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：render_column
** 作用：渲染屏幕的第 i 列：先画天花板色，再画墙体贴图，最后画地板色。
** 参数：
**   game：总结构体（提供帧缓冲 data、贴图、颜色等）
**   r_dir_x, r_dir_y：这一列射线的方向向量
**   i：屏幕列号（0..WIDTH-1）
** 说明：
**   这里直接复用项目里统一的 put_pixel()（不要在本文件重复定义，避免链接冲突）。
*/
void	render_column(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	int				y;
	float			step;
	float			tex_pos;
	int				color;
	t_render_vars	v;

	v = get_render_vars(game, r_dir_x, r_dir_y, i);
	y = -1;
	while (++y < (v.start < 0 ? 0 : v.start))
		put_pixel(v.x, y, game->ceiling_color, game);
	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = (v.start < 0 ? (0 - v.start) : 0) * step;
	y = (v.start < 0) ? 0 : v.start;
	while (y <= (v.end >= HEIGHT ? HEIGHT - 1 : v.end))
	{
		color = *(int *)(v.tex->data + ((int)tex_pos % v.tex->height
					* v.tex->size_line + v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
	y--;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}
