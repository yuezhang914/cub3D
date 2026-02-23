/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:55:20 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_render_vars	get_render_vars(t_game *game, float r_dir_x,
		float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	/* 1. 初始化返回结构体，清零防止垃圾数据 */
	ft_bzero(&v, sizeof(t_render_vars));
	
	/* 2. DDA 初始化：计算步长 (delta)、方向 (step) 和初始边距 (sideDist) */
	ray_init_dda(game, &d, r_dir_x, r_dir_y);
	
	/* 3. 执行 DDA 算法循环：让射线在网格中不断“跳跃”，直到撞到墙壁为止 */
	/* 如果没有撞到任何东西（返回 false），则返回初始化的空变量 */
	if (!ray_run_dda_until_hit(game, &d, r_dir_x, r_dir_y))
		return (v);
	
	/* 4. 转换结果：根据 DDA 撞击点的信息，计算墙面高度、纹理坐标和垂直距离 */
	v = ray_make_render_vars(game, &d, r_dir_x, r_dir_y);
	
	/* 5. 填充 Z-Buffer（深度缓冲）：记录该列光线行进的垂直距离 */
	/* 这对处理精灵（Sprites）的遮挡关系至关重要 */
	game->z_buffer[i] = v.perp_dist;
	
	/* 6. 记录当前处理的屏幕 X 轴坐标 */
	v.x = i;
	return (v);
}
