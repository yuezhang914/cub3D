/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 22:06:08 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：ray_step_once
** 作用：DDA 走“一步”：比较 side_x 和 side_y，选择更近的那条网格线先碰到
**
** 参数：
**   d：DDA 状态（会更新 side_x/side_y、map_x/map_y，并设置 d->side）
**
** 主要逻辑（DDA 核心）：
**   - side_x 表示“从玩家到下一条竖直网格线”的射线长度
**   - side_y 表示“从玩家到下一条水平网格线”的射线长度
**
**   谁更小，说明射线先碰到谁：
**   1) side_x < side_y：
**      - 先碰到竖线 → 进入下一个 x 格子：map_x += step_x
**      - side_x += delta_x（下一次再碰到竖线的距离加一段）
**      - side = 0（表示撞的是竖直边界）
**
**   2) 否则：
**      - 先碰到横线 → map_y += step_y
**      - side_y += delta_y
**      - side = 1（表示撞的是水平边界）
**
** 调用者：
**   ray_run_dda_until_hit()（raycasting_core.c）
**   door_find_open_door()（raycasting_door_overlay.c，BONUS）
*/
void	ray_step_once(t_dda *d)
{
	if (d->side_x < d->side_y)
	{
		d->side_x += d->delta_x;
		d->map_x += d->step_x;
		d->side = 0;
		return ;
	}
	d->side_y += d->delta_y;
	d->map_y += d->step_y;
	d->side = 1;
}

/*
** 函数名：ray_outside_map
** 作用：检查当前 DDA 所在格子是否越界（射线跑出地图直接结束）
**
** 参数：
**   game：提供 map_w/map_h
**   d：提供当前 map_x/map_y
**
** 返回：
**   1：越界
**   0：没越界
**
** 调用者：
**   ray_run_dda_until_hit()
**   door_find_open_door()
*/
int	ray_outside_map(t_game *game, t_dda *d)
{
	if (d->map_x < 0 || d->map_x >= game->map_w)
		return (1);
	if (d->map_y < 0 || d->map_y >= game->map_h)
		return (1);
	return (0);
}

/*
** 函数名：ray_pick_texture
** 作用：根据命中的格子内容与命中面方向，选择要用的贴图（墙四面 or 门）
**
** 参数：
**   game：提供各种贴图：north/south/east/west/door/door_open，以及 door_state
**   d：提供命中位置 map_x/map_y 和 side（竖面/横面）
**   r_dx/r_dy：射线方向（决定是东/西 或 南/北）
**
** 返回：
**   指向应使用的 t_tex
**
** 主要逻辑：
**   1) 如果格子是 'D'（门）：
**      - 如果 door_state[y][x]==1（开门）→ door_open
**      - 否则 → door（关门）
**
**   2) 否则是墙：
**      - side==0：撞竖面（东西墙）
**        r_dx>0 → east，否则 west
**      - side==1：撞横面（南北墙）
**        r_dy>0 → south，否则 north
**
** 调用者：
**   ray_make_render_vars()（raycasting_core.c）
*/
t_tex	*ray_pick_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
	if (game->map[d->map_y][d->map_x] == 'D')
	{
		if (game->door_state && game->door_state[d->map_y][d->map_x] == 1)
			return (&game->door_open);
		return (&game->door);
	}
	if (d->side == 0 && r_dx > 0.0f)
		return (&game->east);
	if (d->side == 0)
		return (&game->west);
	if (r_dy > 0.0f)
		return (&game->south);
	return (&game->north);
}

/*
** 函数名：ray_get_perp_dist
** 作用：从 DDA 状态里计算“命中点距离”（perp_dist）
**
** 参数：
**   d：DDA 状态，里面有 side_x/side_y/delta_x/delta_y/side
**
** 返回：
**   perp_dist：命中点距离
**
** 主要逻辑（关键细节）：
**   DDA 每 step_once 会先把 side_x/side_y 加上 delta 再更新格子，
**   所以命中格子时，side_x/side_y 已经“超前加过一次”，需要减回去一次：
**   - side==0：perp_dist = side_x - delta_x
**   - side==1：perp_dist = side_y - delta_y
**
** 调用者：
**   ray_make_render_vars()（墙/门）
**   door_find_open_door()（开门 overlay）
*/
float	ray_get_perp_dist(t_dda *d)
{
	if (d->side == 0)
		return (d->side_x - d->delta_x);
	return (d->side_y - d->delta_y);
}

/*
** 函数名：ray_calc_tex_x
** 作用：计算贴图横坐标 tex_x（从贴图哪一列取颜色）
**
** 参数：
**   game：提供玩家位置 player.x/y
**   d：提供 side（撞竖面还是横面）
**   tex：贴图（需要它的 width）
**   ray：射线信息（dir_x/dir_y/perp_dist）
**
** 返回：
**   tex_x：范围 [0, tex->width-1]
**
** 主要逻辑（一步一步）：
**   1) 先算“撞击点在墙面上的相对位置 hit”
**      - 若撞竖面（side==0），竖面 x 是整数边界，变化的是 y：
**          hit = player.y + perp_dist * dir_y
**      - 若撞横面（side==1），变化的是 x：
**          hit = player.x + perp_dist * dir_x
**
**   2) hit -= floor(hit)：只保留小数部分（范围 [0,1)）
**      - 表示撞在这一格墙边的“从上到下/从左到右”的比例位置
**
**   3) tex_x = (int)(hit * tex->width)
**      - 把比例映射到贴图宽度像素列
**
**   4) 根据朝向决定是否翻转 tex_x（否则会镜像错）
**      - 撞竖面且射线向左（dir_x<0）→ 翻转
**      - 撞横面且射线向下（dir_y>0）→ 翻转
**
** 调用者：
**   ray_make_render_vars()
**   door_make_overlay_vars()
*/
int	ray_calc_tex_x(t_game *game, t_dda *d, t_tex *tex, t_ray ray)
{
	float	hit;
	int		tex_x;

	if (d->side == 0)
		hit = game->player.y + ray.perp_dist * ray.dir_y;
	else
		hit = game->player.x + ray.perp_dist * ray.dir_x;

	/* 取小数部分：hit ∈ [0,1) */
	hit -= floor((double)hit);
	if (hit < 0)
		hit += 1.0;

	tex_x = (int)(hit * (float)tex->width);

	/* 边界保护 */
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_x < 0)
		tex_x = 0;

	/* 方向翻转：避免贴图左右颠倒 */
	if ((d->side == 0 && ray.dir_x < 0.0) || (d->side == 1 && ray.dir_y > 0.0))
		tex_x = tex->width - tex_x - 1;

	return (tex_x);
}