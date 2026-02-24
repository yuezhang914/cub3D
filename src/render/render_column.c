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

void	draw_door_overlay_column_bonus(t_game *game, float rx, float ry, int i);
#else

void	draw_door_overlay_column_bonus(t_game *game, float rx, float ry, int i)
{
	(void)game;
	(void)rx;
	(void)ry;
	(void)i;
}
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

	/* 1. 计算纹理步进值 (step) */
	/* 意义：屏幕上每移动 1 个像素，对应在纹理图片上需要跳过多少个像素 */
	/* 公式：纹理总高度 / 墙体在屏幕上的像素高度 */
	/* 如果 v.line_h 非常大（离墙近），step 就会很小，纹理被拉伸 */
	step = 1.0f * v.tex->height / v.line_h;

	/* 2. 初始化纹理起始位置 (tex_pos) */
	/* 默认从纹理的最顶端 (0.0) 开始采样 */
	tex_pos = 0.0f;

	/* 3. 裁剪补偿逻辑 (Wall Clipping) */
	/* 如果 v.start < 0，说明墙的顶端已经超出了屏幕上方 */
	/* 为了让屏幕第一行显示正确的纹理，我们需要跳过纹理顶部被“裁掉”的部分 */
	if (v.start < 0)
	{
		/* 计算偏移量：超出的像素数 * 每像素对应的纹理步长 */
		/* 注意：-v.start 是正数，代表超出的像素量 */
		tex_pos = (float)(-v.start) * step;
	}

	/* 4. 执行绘制 */
	/* 将计算好的缩放比和起始位置传入底层绘制函数 */
	draw_wall(v, game, step, tex_pos);
}

void	render_column(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_render_vars	v;

	v = get_render_vars(game, r_dir_x, r_dir_y, i);
	draw_ceiling_part(game, v);
	draw_wall_part(game, v);
	draw_floor_part(game, v);
	draw_door_overlay_column_bonus(game, r_dir_x, r_dir_y, i);
}
