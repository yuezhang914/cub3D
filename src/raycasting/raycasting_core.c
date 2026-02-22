/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 22:06:26 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS
/*
** 函数名：is_hit_bonus（static，BONUS）
** 作用：判断当前 DDA 格子是否算“命中”（需要停止射线）
**
** 参数：
**   game：提供 map、以及门相关逻辑 door_ray_blocks
**   d：当前格子位置 map_x/map_y
**   rx/ry：射线方向（door_ray_blocks 可能要用来判断门是否挡住）
**
** 返回：
**   1：命中（停止）
**   0：未命中（继续走）
**
** 主要逻辑：
**   - 遇到 '1' 墙：一定命中
**   - 遇到 'D' 门：不一定命中
**       door_ray_blocks() 判断这扇门此刻是否挡住射线：
**       - 关门/未完全打开 → 挡住 → 命中
**       - 开门/打开到不挡 → 不命中，射线可以继续穿过去
**
** 调用者：
**   ray_run_dda_until_hit()
*/
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
/*
** 函数名：is_hit_bonus（static，非 BONUS）
** 作用：基础版只有墙：只要遇到 '1' 就命中
**
** 参数：
**   rx/ry：不用，所以用 (void) 消除 unused warning
**
** 调用者：
**   ray_run_dda_until_hit()
*/
static int	is_hit_bonus(t_game *game, t_dda *d, float rx, float ry)
{
	(void)rx;
	(void)ry;
	if (game->map[d->map_y][d->map_x] == '1')
		return (1);
	return (0);
}
#endif

/*
** 函数名：ray_run_dda_until_hit
** 作用：循环执行 DDA：一步一步走格子，直到命中墙/门，或跑出地图
**
** 参数：
**   game：用于越界检测与判断命中
**   d：DDA 状态（会被不断更新）
**   r_dir_x/r_dir_y：射线方向（BONUS 用于门阻挡判断）
**
** 返回：
**   1：成功命中（找到可渲染的物体）
**   0：跑出地图也没命中（一般不会正常发生，或地图有问题）
**
** 主要逻辑：
**   while(1):
**     1) ray_step_once：走一步（跨竖线或横线）
**     2) ray_outside_map：越界就失败返回 0
**     3) is_hit_bonus：命中就返回 1
**
** 调用者：
**   get_render_vars()（raycasting_cal.c）
*/
int	ray_run_dda_until_hit(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	while (1)
	{
		ray_step_once(d);
		if (ray_outside_map(game, d))
			return (0);
		if (is_hit_bonus(game, d, r_dir_x, r_dir_y))
			return (1);
	}
	return (0);
}

/*
** 函数名：ray_make_render_vars
** 作用：根据“已经命中的 DDA 状态”，计算这一列屏幕需要的渲染参数
**
** 参数：
**   game：提供贴图、玩家位置等
**   d：命中时的 DDA 状态（side、map_x/map_y、side_x/side_y 等）
**   r_dir_x/r_dir_y：射线方向
**
** 返回：
**   t_render_vars v：
**     - tex：选中的贴图
**     - perp_dist：距离（用于墙高）
**     - line_h：屏幕上这一列墙要画的高度
**     - start/end：墙柱子在屏幕上的起止 y
**     - tex_x：贴图横坐标
**
** 主要逻辑（数学解释）：
**   1) 选贴图：ray_pick_texture（东/西/南/北 或 门）
**   2) 算距离：ray_get_perp_dist
**      - 并做下限保护：<0.1f 则抬到 0.1f，避免除以接近 0 导致墙高爆炸
**   3) 墙高：line_h = HEIGHT / perp_dist
**      - 近 → perp_dist 小 → line_h 大（墙看起来高）
**      - 远 → perp_dist 大 → line_h 小
**   4) start/end：让墙柱子以屏幕中线为中心上下展开
**   5) tex_x：ray_calc_tex_x
**
** 调用者：
**   get_render_vars()（raycasting_cal.c）
*/
t_render_vars	ray_make_render_vars(t_game *game, t_dda *d, float r_dir_x,
		float r_dir_y)
{
	t_render_vars	v;
	t_ray			ray;

	ft_bzero(&v, sizeof(t_render_vars));

	v.tex = ray_pick_texture(game, d, r_dir_x, r_dir_y);

	v.perp_dist = ray_get_perp_dist(d);
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;

	/* 组装一个 ray 结构给 tex_x 计算用 */
	ray.perp_dist = v.perp_dist;
	ray.dir_x = r_dir_x;
	ray.dir_y = r_dir_y;

	v.line_h = (int)(HEIGHT / v.perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);

	v.tex_x = ray_calc_tex_x(game, d, v.tex, ray);
	return (v);
}
