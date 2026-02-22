/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_door_overlay.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 12:53:28 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:07:10 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

/*
** 函数名：door_find_open_door（static，BONUS）
** 作用：沿射线 DDA 往前找：寻找第一个“开着的门 'D'”
**
** 参数：
**   game：提供 map、door_state
**   d：DDA 状态（会被一步步更新；找到门后会写入 d->perp_dist）
**   r_dir_x/r_dir_y：射线方向（door_ray_blocks 需要）
**
** 返回：
**   1：找到了“开门”（door_state==1），并设置了 d->perp_dist
**   0：失败（越界/先遇墙/遇到关门或未开门）
**
** 主要逻辑：
**   while(1):
**     1) ray_step_once
**     2) 越界 → 0
**     3) 遇墙 '1' → 0（墙挡在前面，门不可能叠加在墙前）
**     4) 遇门 'D':
**        - door_state==1 → 成功：
**            d->perp_dist = ray_get_perp_dist(d)
**            调一次 door_ray_blocks(...)（这里作者用它同步/更新一些门的细节）
**            return 1
**        - 否则 → 0
**
** 调用者：
**   get_open_door_overlay_vars()
*/
static int	door_find_open_door(t_game *game, t_dda *d,
					float r_dir_x, float r_dir_y)
{
	while (1)
	{
		ray_step_once(d);
		if (ray_outside_map(game, d))
			return (0);
		if (game->map[d->map_y][d->map_x] == '1')
			return (0);
		if (game->map[d->map_y][d->map_x] == 'D')
		{
			if (game->door_state && game->door_state[d->map_y][d->map_x] == 1)
			{
				d->perp_dist = ray_get_perp_dist(d);
				(void)door_ray_blocks(game, d, r_dir_x, r_dir_y);
				return (1);
			}
			return (0);
		}
	}
	return (0);
}

/*
** 函数名：door_make_overlay_vars（static，BONUS）
** 作用：把“开门命中信息”转换成 overlay 的渲染参数（贴图固定是 door_open）
**
** 参数：
**   game：提供 door_open 贴图、玩家位置
**   d：命中开门后的 DDA 状态（里面有 d->perp_dist 和 d->side 等）
**   r_dir_x/r_dir_y：射线方向（算 tex_x 用）
**
** 返回：
**   t_render_vars v（用于画门框叠加这一列）
**
** 主要逻辑：
**   与 ray_make_render_vars 很像，但 tex 固定为 door_open，距离直接取 d->perp_dist
**
** 调用者：
**   get_open_door_overlay_vars()
*/
static t_render_vars	door_make_overlay_vars(t_game *game, t_dda *d,
						float r_dir_x, float r_dir_y)
{
	t_render_vars	v;
	t_ray			ray;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = &game->door_open;

	v.perp_dist = d->perp_dist;
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;

	ray.perp_dist = v.perp_dist;
	ray.dir_x = r_dir_x;
	ray.dir_y = r_dir_y;

	v.line_h = (int)(HEIGHT / v.perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);

	v.tex_x = ray_calc_tex_x(game, d, v.tex, ray);
	return (v);
}

/*
** 函数名：get_open_door_overlay_vars（BONUS）
** 作用：计算“开门门框叠加（overlay）”这一列的渲染参数
**
** 参数：
**   game：提供 door_state、door_open、玩家位置等
**   r_dir_x/r_dir_y：这一列射线方向
**   i：屏幕列下标
**
** 返回：
**   t_render_vars v：
**     - 若没有开门可叠加 → 返回全 0 的 v（表示这一列不画门框）
**     - 有则返回完整参数，并设置 v.x=i
**
** 主要逻辑：
**   1) 如果 game/door_state/door_open.data 不可用 → 直接返回空
**   2) ray_init_dda 初始化 d
**   3) door_find_open_door：沿射线找第一个开门
**   4) door_make_overlay_vars：生成 overlay 参数（tex=door_open）
**   5) v.x=i
**
** 调用者：
**   draw_door_overlay_column_bonus()（你别的文件里：画完墙后叠加这一层）
*/
t_render_vars	get_open_door_overlay_vars(t_game *game, float r_dir_x,
						float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	if (!game || !game->door_state || !game->door_open.data)
		return (v);

	ray_init_dda(game, &d, r_dir_x, r_dir_y);

	if (!door_find_open_door(game, &d, r_dir_x, r_dir_y))
		return (v);

	v = door_make_overlay_vars(game, &d, r_dir_x, r_dir_y);
	v.x = i;
	return (v);
}

#endif