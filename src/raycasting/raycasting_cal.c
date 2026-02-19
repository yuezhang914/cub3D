/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:00:22 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * 初始化 DDA 算法所需的步进方向和初始边距
 */
static void	init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;
	d->delta_x = (r_dir_x == 0) ? 1e30 : fabsf(1.0f / r_dir_x);
	d->delta_y = (r_dir_y == 0) ? 1e30 : fabsf(1.0f / r_dir_y);
	if (r_dir_x < 0)
	{
		d->step_x = -1;
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
	}
	else
	{
		d->step_x = 1;
		d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
	}
	if (r_dir_y < 0)
	{
		d->step_y = -1;
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
	}
	else
	{
		d->step_y = 1;
		d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
	}
}

/*
** 函数：perform_dda（static）
** 作用：执行 DDA 步进，直到命中墙/门板，或越界失败。
** 参数：
**   game：总结构体
**   d：DDA 状态（会被不断更新）
**   r_dir_x/r_dir_y：射线方向（用于门命中判断 door_ray_blocks）
** 返回：
**   1=命中可渲染物（墙/门板）
**   0=越界（射线跑出地图）
** 调用：
**   get_render_vars()
*/
/*
** 函数：perform_dda（static）
** 作用：执行 DDA 步进，直到命中墙（BONUS 时也可能命中门板）或越界
** 参数：
**   game：总结构体
**   d：DDA 状态
**   r_dir_x/r_dir_y：射线方向（BONUS 门判断用；mandatory 下会被忽略）
*/
static int  perform_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
    /* mandatory 下没用到射线方向，避免 -Werror unused-parameter */
#ifndef BONUS
    (void)r_dir_x;
    (void)r_dir_y;
#endif

    while (1)
    {
        if (d->side_x < d->side_y)
        {
            d->side_x += d->delta_x;
            d->map_x += d->step_x;
            d->side = 0;
        }
        else
        {
            d->side_y += d->delta_y;
            d->map_y += d->step_y;
            d->side = 1;
        }
        if (d->map_x < 0 || d->map_x >= game->map_w || d->map_y < 0
            || d->map_y >= game->map_h)
            return (0);
        if (game->map[d->map_y][d->map_x] == '1')
            return (1);

#ifdef BONUS
        if (game->map[d->map_y][d->map_x] == 'D')
        {
            /* 命中门格子：先算 perp_dist（和墙一样） */
            if (d->side == 0)
                d->perp_dist = (d->side_x - d->delta_x);
            else
                d->perp_dist = (d->side_y - d->delta_y);

            /* 门：命中门板就停；命中门洞就继续 DDA */
            if (door_ray_blocks(game, d, r_dir_x, r_dir_y))
                return (1);
        }
#endif
    }
    return (1);
}


/*
** 函数：get_texture（static）
** 作用：根据命中格子类型 + 命中面，返回对应纹理指针
** 参数：
**   game：总结构体（取纹理地址）
**   d：DDA 状态（side 表示撞到竖/横边）
**   r_dx, r_dy：射线方向
** 返回：
**   对应纹理指针（墙：NO/SO/WE/EA；门：door）
** 调用：
**   calculate_render_vars()
*/
static t_tex	*get_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
#ifdef BONUS
	if (game->map[d->map_y][d->map_x] == 'D')
		return (&game->door);
#endif
	if (d->side == 0)
	{
		if (r_dx > 0)
			return (&game->east);
		return (&game->west);
	}
	else
	{
		if (r_dy > 0)
			return (&game->south);
		return (&game->north);
	}
}

/**
 * 计算垂直距离、纹理坐标并填充渲染结构体
 */
static t_render_vars	calculate_render_vars(t_game *game, t_dda *d, int i,
		float r_dir[2])
{
	t_render_vars	v;
	float			wall_hit;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = get_texture(game, d, r_dir[0], r_dir[1]);
	if (!v.tex)
		return (v);
	if (d->side == 0)
		d->perp_dist = d->side_x - d->delta_x;
	else
		d->perp_dist = d->side_y - d->delta_y;
	if (d->perp_dist < 0.1f)
		d->perp_dist = 0.1f;
	game->z_buffer[i] = d->perp_dist;
	v.line_h = (int)(HEIGHT / d->perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);
	v.x = i;
	if (d->side == 0)
		wall_hit = game->player.y + d->perp_dist * r_dir[1];
	else
		wall_hit = game->player.x + d->perp_dist * r_dir[0];
	wall_hit -= floor(wall_hit);
	v.tex_x = (int)(wall_hit * (float)v.tex->width);
	/* 关键修正：确保索引永远不会等于或超过 width */
	if (v.tex_x >= v.tex->width)
		v.tex_x = v.tex->width - 1;
	if (v.tex_x < 0)
		v.tex_x = 0;
	if ((d->side == 0 && r_dir[0] < 0) || (d->side == 1 && r_dir[1] > 0))
		v.tex_x = v.tex->width - v.tex_x - 1;
	return (v);
}

/**
 * 核心射线投射函数
 */
t_render_vars	get_render_vars(t_game *game, float r_dir_x, float r_dir_y,
		int i)
{
	t_dda			d;
	t_render_vars	v;
	float			r_dir[2];

	ft_bzero(&v, sizeof(t_render_vars));
	r_dir[0] = r_dir_x;
	r_dir[1] = r_dir_y;
	init_dda(game, &d, r_dir_x, r_dir_y);
	init_dda(game, &d, r_dir_x, r_dir_y);
	if (!perform_dda(game, &d, r_dir_x, r_dir_y))
		return (v);
	v = calculate_render_vars(game, &d, i, r_dir);
	return (v);
}
