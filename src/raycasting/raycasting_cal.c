/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:55:20 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


/*
** 函数：get_render_vars
** 作用：对第 i 列射线做 DDA 命中，计算该列应该如何画（纹理、高度、纹理坐标等）
** 参数：
**   game：游戏总结构体
**   r_dir_x/r_dir_y：射线方向
**   i：屏幕列号
** 逻辑：
**   1) 初始化 DDA
**   2) 执行 DDA 直到命中墙/门板
**   3) 计算该列的渲染参数并返回
** 在哪调用：
**   render_column() 或 draw_walls() 每列调用一次
*/
t_render_vars	get_render_vars(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	ray_init_dda(game, &d, r_dir_x, r_dir_y);
	if (!ray_run_dda_until_hit(game, &d, r_dir_x, r_dir_y))
		return (v);
	v = ray_make_render_vars(game, &d, r_dir_x, r_dir_y);
	game->z_buffer[i] = v.perp_dist;
	v.x = i;
	return (v);
}
