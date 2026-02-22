/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 22:06:55 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：get_render_vars
** 作用：对“屏幕第 i 列”这条射线，完整跑一遍：
**       init dda → run dda → 计算渲染参数 → 写入 z_buffer[i]
**
** 参数：
**   game：提供玩家/地图/贴图，并写入 z_buffer
**   r_dir_x / r_dir_y：这一列射线的方向
**   i：屏幕列下标（0..WIDTH-1）
**
** 返回：
**   t_render_vars v：
**     - 若没命中，返回全 0 的 v（tex=NULL、perp_dist=0 等）
**     - 命中则返回完整渲染参数（并设置 v.x=i）
**
** 主要逻辑：
**   1) 清零 v（默认返回空）
**   2) ray_init_dda：初始化 DDA 状态 d
**   3) ray_run_dda_until_hit：沿射线走格子直到命中
**      - 若失败：直接返回空 v
**   4) ray_make_render_vars：把命中信息转成渲染参数
**   5) z_buffer[i] = perp_dist：记录这一列最近墙/门距离
**      - 之后画 sprite 或门叠加时，用来判断“谁在前谁在后”
**   6) v.x = i
**
** 调用者：
**   （通常是渲染主循环里）对每一列 i 调一次，拿到参数去画墙柱
*/
t_render_vars	get_render_vars(t_game *game, float r_dir_x,
		float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));

	ray_init_dda(game, &d, r_dir_x, r_dir_y);

	if (!ray_run_dda_until_hit(game, &d, r_dir_x, r_dir_y))
		return (v);

	v = ray_make_render_vars(game, &d, r_dir_x, r_dir_y);

	/* 记录这一列“最近命中物”的距离，用于遮挡判断 */
	game->z_buffer[i] = v.perp_dist;

	v.x = i;
	return (v);
}