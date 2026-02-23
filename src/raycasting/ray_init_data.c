/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_init_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 18:10:16 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 18:10:18 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ray_init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	/* 1. 将玩家的浮点数坐标转换为整数，确定当前光线所在的网格坐标（格子） */
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;

	/* 2. 计算 delta_x：光线从一条垂直网格线跨越到下一条垂直网格线所经过的长度 */
	/* 如果 r_dir_x 为 0，说明光线与垂直线平行，永远不会相交，距离设为无穷大 */
	if (r_dir_x == 0.0f)
		d->delta_x = (float)INF;
	else
		d->delta_x = fabsf(1.0f / r_dir_x);

	/* 3. 计算 delta_y：光线从一条水平网格线跨越到下一条水平网格线所经过的长度 */
	if (r_dir_y == 0.0f)
		d->delta_y = (float)INF;
	else
		d->delta_y = fabsf(1.0f / r_dir_y);

	/* 4. 计算步进方向 (Step) 和 初始边距 (SideDist) */
	/* step_x 会决定是向左(-1)还是向右(+1)跳格 */
	/* side_dist_x 是光线从起点到遇到的第一条 X 方向网格线的距离 */
	ray_set_step_x(game, d, r_dir_x);
	ray_set_step_y(game, d, r_dir_y);
}

void	ray_set_step_x(t_game *game, t_dda *d, float r_dir_x)
{
	/* 1. 如果光线的 X 分量小于 0，说明光线正在向左（西）飞行 */
	if (r_dir_x < 0.0f)
	{
		/* 步进方向设为 -1，即每次 DDA 移动都向左跳一格网格 */
		d->step_x = -1;
		/* 计算 side_x：玩家坐标到当前格子左边界的距离，再乘以单位步长 delta_x */
		/* (game->player.x - d->map_x) 得到的是玩家在格子内的相对位置偏移量 */
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
		return ;
	}
	/* 2. 如果光线的 X 分量大于等于 0，说明光线正在向右（东）飞行 */
	d->step_x = 1;
	/* 计算 side_x：当前格子右边界（d->map_x + 1.0）到玩家坐标的距离 */
	/* 乘以 delta_x 得到光线斜着走到右侧格线所需的实际路程长度 */
	d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
}

void	ray_set_step_y(t_game *game, t_dda *d, float r_dir_y)
{
	/* 1. 如果光线的 Y 分量小于 0，说明光线在地图上是“向上”飞的 */
	/* 在 cub3D 的坐标系中，y 减小通常代表向屏幕上方（北）移动 */
	if (r_dir_y < 0.0f)
	{
		/* 步进方向设为 -1，即 DDA 循环每次向上跳一个格子 */
		d->step_y = -1;
		/* 计算 side_y：玩家 Y 坐标到当前格子顶边（上边界）的垂直距离 */
		/* (game->player.y - d->map_y) 得到玩家在格子内部的相对垂直偏移 */
		/* 乘以 delta_y 转换为光线在斜线上行进的实际长度 */
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
		return ;
	}
	/* 2. 如果光线的 Y 分量大于等于 0，说明光线正在“向下”（南）飞 */
	d->step_y = 1;
	/* 计算 side_y：当前格子底边（d->map_y + 1.0）到玩家 Y 坐标的垂直距离 */
	/* 乘以 delta_y 得到光线斜着走到下侧格线所需的实际长度 */
	d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
}