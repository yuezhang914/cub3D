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

/**
 * 执行 DDA 核心循环，直到碰撞墙壁或超出地图边界
 */
static int	perform_dda(t_game *game, t_dda *d)
{
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
		if (game->map[d->map_y][d->map_x] == '1'
			|| game->map[d->map_y][d->map_x] == 'D')
			break ;
	}
	return (1);
}

/**
 * 根据射线方向和碰撞平面选择对应的纹理
 * 作用：判断碰撞的是哪一面墙（东西或南北），并根据射线向量的正负
 * 确定具体的方位（东、西、南、北），返回对应的纹理指针。
 */
static t_tex	*get_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
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
	if (!perform_dda(game, &d))
		return (v);
	v = calculate_render_vars(game, &d, i, r_dir);
	return (v);
}
