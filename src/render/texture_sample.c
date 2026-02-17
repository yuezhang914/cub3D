/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sample.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:04:51 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 01:13:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static int		clamp_int(int v, int lo, int hi);
static float		wall_rel_x(t_coords wall_hit, t_dir dir);
static t_tex		*wall_texture(t_game *game, t_dir dir);

/*
** 函数：tex_sample
** 作用：从纹理 tex 里按“相对坐标 uv(0..1)”取一个像素颜色。
** 参数：
**   tex：纹理（必须已经加载过 data/width/height）
**   uv ：相对坐标（0..1），uv.x 控制左右，uv.y 控制上下
** 返回：
**   颜色 int
** 用在哪：
**   画墙贴图、缩放贴图、动画叠加等。
*/
int	tex_sample(t_tex *tex, t_coords uv)
{
	int	x;
	int	y;
	int	offset;

	x = (int)(uv.x * tex->width);
	y = (int)(uv.y * tex->height);
	x = clamp_int(x, 0, tex->width - 1);
	y = clamp_int(y, 0, tex->height - 1);
	offset = y * tex->size_line + x * (tex->bpp / 8);
	return (*(int *)((char *)tex->data + offset));
}

/*
** 函数：wall_tex_color
** 作用：根据射线命中点 wall_hit + 墙上相对高度 rel_y(0..1)，选对方向纹理并取颜色。
** 参数：
**   game    ：提供四向纹理 north/east/south/west
**   wall_hit：射线撞墙坐标
**   rel_y   ：墙上的相对高度（0..1）
** 返回：
**   颜色 int
** 用在哪：
**   draw_walls() 画每一列墙的时候使用。
*/
int	wall_tex_color(t_game *game, t_coords wall_hit, float rel_y)
{
	t_dir		dir;
	t_coords	uv;
	t_tex		*tex;

	dir = hit_direction(game, wall_hit);
	uv.x = wall_rel_x(wall_hit, dir);
	uv.y = rel_y;
	tex = wall_texture(game, dir);
	return (tex_sample(tex, uv));
}

static int	clamp_int(int v, int lo, int hi)
{
	if (v < lo)
		return (lo);
	if (v > hi)
		return (hi);
	return (v);
}

static float	wall_rel_x(t_coords wall_hit, t_dir dir)
{
	float	dummy;
	float	f;

	if (dir == north)
		f = modff(wall_hit.x, &dummy);
	else if (dir == east)
		f = modff(wall_hit.y, &dummy);
	else if (dir == south)
		f = 1.0f - modff(wall_hit.x, &dummy);
	else
		f = 1.0f - modff(wall_hit.y, &dummy);
	return (f);
}

static t_tex	*wall_texture(t_game *game, t_dir dir)
{
	if (dir == north)
		return (&game->north);
	if (dir == east)
		return (&game->east);
	if (dir == south)
		return (&game->south);
	return (&game->west);
}
