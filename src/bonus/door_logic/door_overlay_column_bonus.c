/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_overlay_column_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:37:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 13:37:36 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"


static void	draw_overlay_transparent(t_render_vars v, t_game *game,
				float step, float tex_pos)
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
		if (color != 0)
			put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
}

/*
** 函数：draw_door_overlay_column_bonus
** 作用：BONUS：叠加绘制“开门门框”，让门中间透明能看到后面的墙/巷子
** 参数：
**   game：游戏主结构体（包含 z_buffer、贴图数据、画布）
**   r_dir_x：当前列射线方向 x
**   r_dir_y：当前列射线方向 y
**   i：屏幕列下标（用于 z_buffer[i]）
** 逻辑：
**   1) get_open_door_overlay_vars：查找该列第一个“开着的门”，算出门框渲染参数
**   2) 若门在墙前（door_dist < z_buffer[i]），则叠加画门框
**   3) 颜色为 0 当作透明，跳过不画（露出底下墙面）
** 在哪调用：render_column() 的 BONUS 分支（画完墙后）
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
	if (door_v.perp_dist >= game->z_buffer[i])
		return ;
	step = 1.0f * door_v.tex->height / door_v.line_h;
	tex_pos = 0.0f;
	if (door_v.start < 0)
		tex_pos = (float)(-door_v.start) * step;
	draw_overlay_transparent(door_v, game, step, tex_pos);
}
