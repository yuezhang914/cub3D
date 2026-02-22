/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_init_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 18:10:16 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 22:05:49 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：ray_init_dda
** 作用：初始化 DDA（走格子算法）需要的所有基础数据，让射线能“一步步在格子里走”
**
** 参数：
**   game：提供玩家位置 game->player.x/y
**   d：DDA 状态结构体（本函数会填充它的字段）
**   r_dir_x / r_dir_y：射线方向向量（这一列屏幕对应的视线方向）
**
** 主要逻辑（数学解释）：
**   1) map_x/map_y：玩家所在的格子坐标（对玩家小数坐标取 int）
**      - 例如 player.x=3.5 → map_x=3（第3列格子）
**
**   2) delta_x = abs(1 / r_dir_x)
**      delta_y = abs(1 / r_dir_y)
**      - 这是“射线每跨过一条竖直/水平网格线，射线长度要增加多少”
**      - 如果 r_dir_x==0，说明射线完全不往 x 方向走，那么跨竖线距离视为 INF
**
**   3) 再分别调用 ray_set_step_x / ray_set_step_y：
**      - 决定 step_x/step_y（+1 还是 -1）
**      - 决定 side_x/side_y（第一次碰到竖线/横线的距离）
**
** 调用者：
**   get_render_vars()（raycasting_cal.c）
**   get_open_door_overlay_vars()（raycasting_door_overlay.c，BONUS）
*/
void	ray_init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;

	/* delta_x：跨过一条“竖直网格线”所需要的射线长度增量 */
	if (r_dir_x == 0.0f)
		d->delta_x = (float)INF;
	else
		d->delta_x = fabsf(1.0f / r_dir_x);

	/* delta_y：跨过一条“水平网格线”所需要的射线长度增量 */
	if (r_dir_y == 0.0f)
		d->delta_y = (float)INF;
	else
		d->delta_y = fabsf(1.0f / r_dir_y);

	/* 计算 x/y 方向分别怎么走、第一次要走多远才能碰到下一条网格线 */
	ray_set_step_x(game, d, r_dir_x);
	ray_set_step_y(game, d, r_dir_y);
}

/*
** 函数名：ray_set_step_x
** 作用：设置 DDA 在 x 方向的步进方向 step_x，并计算第一次 side_x
**
** 参数：
**   game：提供玩家 x 坐标
**   d：要写入 step_x、side_x、以及会用到 map_x/delta_x
**   r_dir_x：射线 x 方向分量（决定向左还是向右）
**
** 主要逻辑（非常关键）：
**   - 如果 r_dir_x < 0：射线朝左 → step_x = -1
**     第一次到“左边那条竖线”的距离：
**       (player.x - map_x) * delta_x
**     解释：player.x 在当前格子里距离左边界的水平距离是 (player.x - map_x)
**
**   - 否则：射线朝右 → step_x = +1
**     第一次到“右边那条竖线”的距离：
**       (map_x + 1 - player.x) * delta_x
**
** 调用者：
**   ray_init_dda()
*/
void	ray_set_step_x(t_game *game, t_dda *d, float r_dir_x)
{
	if (r_dir_x < 0.0f)
	{
		d->step_x = -1;
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
		return ;
	}
	d->step_x = 1;
	d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
}

/*
** 函数名：ray_set_step_y
** 作用：设置 DDA 在 y 方向的步进方向 step_y，并计算第一次 side_y
**
** 参数：
**   game：提供玩家 y 坐标
**   d：要写入 step_y、side_y、以及会用到 map_y/delta_y
**   r_dir_y：射线 y 方向分量（决定向上还是向下）
**
** 主要逻辑：
**   - r_dir_y < 0：向上 → step_y=-1
**     第一次到“上边那条横线”的距离：
**       (player.y - map_y) * delta_y
**
**   - 否则：向下 → step_y=+1
**     第一次到“下边那条横线”的距离：
**       (map_y + 1 - player.y) * delta_y
**
** 调用者：
**   ray_init_dda()
*/
void	ray_set_step_y(t_game *game, t_dda *d, float r_dir_y)
{
	if (r_dir_y < 0.0f)
	{
		d->step_y = -1;
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
		return ;
	}
	d->step_y = 1;
	d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
}
