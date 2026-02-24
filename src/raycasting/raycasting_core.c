/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 13:04:44 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

static int	is_hit_bonus(t_game *game, t_dda *d, float rx, float ry)
{
	char	tile;

	tile = game->map[d->map_y][d->map_x];
	if (tile == '1')
		return (1);
	if (tile == 'D')
	{
		if (door_ray_blocks(game, d, rx, ry))
			return (1);
	}
	return (0);
}
#else

static int	is_hit_bonus(t_game *game, t_dda *d, float rx, float ry)
{
	(void)rx;
	(void)ry;
	if (game->map[d->map_y][d->map_x] == '1')
		return (1);
	return (0);
}
#endif

int	ray_run_dda_until_hit(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	/* 1. 进入死循环，让射线开始在网格中前进 */
	while (1)
	{
		/* 2. 核心步进逻辑：比较 side_x 和 side_y，哪边近就往哪边跳一格 */
		/* 该函数会更新 d->map_x/y 和 d->side_dist_x/y */
		ray_step_once(d);

		/* 3. 边界检查：如果射线跳出了地图数组的范围，说明没撞到墙，直接返回失败 */
		if (ray_outside_map(game, d))
			return (0);

		/* 4. 碰撞检查：检查当前格子 [map_y][map_x] 是否为墙 ('1') 或其他可碰撞体 */
		/* 在 BONUS 模式下，这个函数可能还会处理门、精灵等特殊判断 */
		if (is_hit_bonus(game, d, r_dir_x, r_dir_y))
			return (1);
	}
	return (0);
}
t_render_vars	ray_make_render_vars(t_game *game, t_dda *d, float r_dir_x,
		float r_dir_y)
{
	t_render_vars	v;
	t_ray			ray;

	/* 1. 初始化结构体，防止出现垃圾值 */
	ft_bzero(&v, sizeof(t_render_vars));

	/* 2. 纹理选择：根据 DDA 碰撞面（东西南北）选择正确的墙壁贴图 */
	v.tex = ray_pick_texture(game, d, r_dir_x, r_dir_y);

	/* 3. 距离修正：获取垂直墙面的距离 (Perpendicular Wall Distance) */
	/* 注意：不直接使用欧几里得距离是为了防止“鱼眼效应”(Fish-eye effect) */
	v.perp_dist = ray_get_perp_dist(d);

	/* 4. 安全保护：防止距离过近导致 line_h 无穷大或除以零崩溃 */
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;

	/* 封装射线数据，用于后续纹理坐标计算 */
	ray.perp_dist = v.perp_dist;
	ray.dir_x = r_dir_x;
	ray.dir_y = r_dir_y;

	/* 5. 投影计算：计算这一列墙在屏幕上的像素高度 */
	v.line_h = (int)(HEIGHT / v.perp_dist);

	/* 6. 确定绘制范围：计算墙体在屏幕 Y 轴上的起点和终点 */
	/* (HEIGHT / 2) 是视平线，墙体以此为中心上下对称分布 */
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);

	/* 7. 纹理映射：计算当前射线击中点对应的纹理 X 坐标 (0 到 tex->width 之间) */
	v.tex_x = ray_calc_tex_x(game, d, v.tex, ray);

	return (v);
}
