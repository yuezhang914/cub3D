/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_view_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 23:10:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:56:00 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：load_door_texture
** 作用：给 door / door_open 贴图设置路径并加载 xpm
**
** 参数：
**   game：提供 game->door / game->door_open，并提供 load_texture 所需 mlx
**
** 主要逻辑：
**   1) door.path = "assets/door/door_close.xpm"
**      load_texture(game, &door)
**   2) door_open.path = "assets/door/door_open.xpm"
**      load_texture(game, &door_open)
**
** 调用者（典型）：
**   bonus 的 setup_mlx 或 handle_bonus_setup（初始化阶段）
*/
void	load_door_texture(t_game *game)
{
	if (!game)
		return ;
	game->door.path = "assets/door/door_close.xpm";
	load_texture(game, &game->door);
	game->door_open.path = "assets/door/door_open.xpm";
	load_texture(game, &game->door_open);
}

/*
** 函数名：door_ray_blocks
** 作用：当射线走到一个门格子 'D' 时，判断“这扇门此刻是否挡住射线”
**
** 参数：
**   game：提供 door_state、player.x/y
**   d：DDA 状态（提供 map_x/map_y、side、perp_dist）
**   rdx,rdy：射线方向（用于计算撞击点 wall_x）
**
** 返回：
**   1：门挡住射线（射线应在这里停下，渲染门/墙）
**   0：门不挡射线（射线可以穿过去，继续找后面的墙）
**
** 主要逻辑：
**   1) door_state 不存在：保守起见，返回 1（当成挡）
**   2) open = door_state[y][x]（转 float）
**      - open <= 0：关门 → 挡（返回 1）
**      - open >= 1：完全开 → 不挡（返回 0）
**      - 0 < open < 1：半开门（预留动画）
**
**   3) 半开门时要判断射线打在门的哪一部分：
**      - 先算撞击点在门平面上的坐标 wall_x（取小数部分，范围[0,1)）
**        * 如果 d->side==0（撞竖面），变化的是 y：
**            wall_x = player.y + perp_dist * rdy
**        * 否则（撞横面），变化的是 x：
**            wall_x = player.x + perp_dist * rdx
**        然后 wall_x = wall_x - floor(wall_x) 取小数部分
**
**      - 决策：
**        如果 wall_x < open：说明射线打在“已经打开的区域” → 不挡（0）
**        否则：打在“还没打开的区域” → 挡（1）
**
** 抽象概念解释（初中版）：
**   open 是门打开的比例（0=全关，1=全开）
**   wall_x 是射线打在门上的位置（从左到右 0~1）
**   打在已打开部分就穿过去，打在未打开部分就被门挡住
**
** 调用者：
**   raycasting_core.c 的 is_hit_bonus()：射线遇到 'D' 时用它判断是否命中
*/
int	door_ray_blocks(t_game *game, t_dda *d, float rdx, float rdy)
{
	float	open;
	float	wall_x;

	if (!game || !game->door_state)
		return (1);
	open = (float)game->door_state[d->map_y][d->map_x];
	if (open <= 0.0f)
		return (1);
	if (open >= 1.0f)
		return (0);

	/* 半开门：计算射线打在门平面上的位置（取小数部分） */
	if (d->side == 0)
		wall_x = game->player.y + d->perp_dist * rdy;
	else
		wall_x = game->player.x + d->perp_dist * rdx;
	wall_x = wall_x - floorf(wall_x);

	if (wall_x < open)
		return (0);
	return (1);
}
