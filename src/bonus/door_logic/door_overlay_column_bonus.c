/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_overlay_column_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:37:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:56:24 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：draw_overlay_transparent（static）
** 作用：按“透明规则”绘制 overlay 柱子（门开着的贴图叠加）
**
** 参数：
**   v：这一列 overlay 的渲染参数（tex/tex_x/start/end/line_h/x 等）
**   game：画布（put_pixel 写入）
**   step：贴图 y 每画屏幕 1 像素要走多少（tex_h / line_h）
**   tex_pos：起始贴图 y（浮点），用于从正确位置开始采样
**
** 主要逻辑：
**   与普通 draw_wall 类似，但多了“透明判断”：
**   1) y 从 v.start 开始（<0 则从 0）
**   2) tex_y = (int)tex_pos % tex_h
**   3) color = 贴图像素(tex_x, tex_y)
**   4) 如果 color 是透明背景色（0x00000000 或 0xFF000000）就跳过不画
**      否则 put_pixel 画到屏幕
**   5) tex_pos += step，y++
**
** 为什么要跳过这两个颜色？
**   这相当于“颜色键透明”：贴图里用特定黑色/透明黑当作背景，
**   overlay 只画门框/门面，背景不盖住墙。
**
** 调用者：
**   draw_door_overlay_column_bonus()
*/
static void	draw_overlay_transparent(t_render_vars v, t_game *game, float step,
		float tex_pos)
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
		color = *(int *)(v.tex->data + (tex_y * v.tex->size_line + v.tex_x
					* (v.tex->bpp / 8)));

		/* 透明色键：跳过背景，不覆盖墙 */
		if ((unsigned int)color != 0x00000000u
			&& (unsigned int)color != 0xFF000000u)
			put_pixel(v.x, y, color, game);

		tex_pos += step;
		y++;
	}
}

/*
** 函数名：draw_door_overlay_column_bonus
** 作用：对屏幕第 i 列，尝试绘制“开门 overlay”（如果该列前方有开门且在墙前面）
**
** 参数：
**   game：提供 z_buffer（墙距离），以及画布写像素
**   r_dir_x/r_dir_y：该列射线方向（用于计算 overlay 的 render_vars）
**   i：屏幕列号
**
** 主要逻辑：
**   1) door_v = get_open_door_overlay_vars(game, r_dir_x, r_dir_y, i)
**      - 如果没找到开门，door_v.tex 会是 NULL
**   2) 如果 door_v.tex 或 door_v.tex->data 不存在：return
**   3) 深度判断（遮挡关系）：
**      - 如果 door_v.perp_dist >= game->z_buffer[i]：
**        说明这扇开门在墙后面或与墙同距离 → 不应覆盖墙 → return
**      - 否则门更近 → 允许叠加
**   4) 计算 step 与 tex_pos（与画墙相同）
**   5) draw_overlay_transparent：按透明规则画出来
**
** 调用者：
**   render_column.c：在画完墙/地板/天花板之后调用它叠加门
*/
void	draw_door_overlay_column_bonus(t_game *game, float r_dir_x,
		float r_dir_y, int i)
{
	t_render_vars	door_v;
	float			step;
	float			tex_pos;

	door_v = get_open_door_overlay_vars(game, r_dir_x, r_dir_y, i);
	if (!door_v.tex || !door_v.tex->data)
		return ;

	/* 门必须在墙前面（更小距离）才画，否则会错误覆盖墙 */
	if (door_v.perp_dist >= game->z_buffer[i])
		return ;

	step = 1.0f * door_v.tex->height / door_v.line_h;
	tex_pos = 0.0f;
	if (door_v.start < 0)
		tex_pos = (float)(-door_v.start) * step;

	draw_overlay_transparent(door_v, game, step, tex_pos);
}
