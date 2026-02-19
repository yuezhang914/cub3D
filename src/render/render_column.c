/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>               +#+  +:+ 	+#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:56 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:01:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS
void	draw_door_overlay_column_bonus(t_game *game, float r_dir_x,
			float r_dir_y, int i);
#endif

static void	draw_ceiling_part(t_game *game, t_render_vars v)
{
	int	y;

	y = 0;
	while (y < v.start && y < HEIGHT)
	{
		put_pixel(v.x, y, game->ceiling_color, game);
		y++;
	}
}

static void	draw_floor_part(t_game *game, t_render_vars v)
{
	int	y;

	y = v.end;
	if (y < 0)
		y = -1;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}

static void	draw_wall_part(t_game *game, t_render_vars v)
{
	float	step;
	float	tex_pos;

	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = 0.0f;
	if (v.start < 0)
		tex_pos = (float)(-v.start) * step;
	draw_wall(v, game, step, tex_pos);
}

/*
** 函数：render_column
** 作用：渲染屏幕第 i 列（天花板 → 墙 → 地板）
** 参数：
**   game：游戏主结构体（包含画布、颜色、贴图等）
**   r_dir_x：当前列射线方向 x
**   r_dir_y：当前列射线方向 y
**   i：屏幕列下标（0..WIDTH-1）
** 逻辑：
**   1) 调用 get_render_vars 计算该列墙体渲染参数（并写 z_buffer）
**   2) 画天花板、墙、地板
**   3) BONUS：叠加门框（透明像素跳过）
** 在哪调用：draw_walls() 中逐列调用
*/
void	render_column(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_render_vars	v;

	v = get_render_vars(game, r_dir_x, r_dir_y, i);
	draw_ceiling_part(game, v);
	draw_wall_part(game, v);
	draw_floor_part(game, v);
#ifdef BONUS
	draw_door_overlay_column_bonus(game, r_dir_x, r_dir_y, i);
#endif
}
