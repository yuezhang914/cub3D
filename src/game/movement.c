/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 10:13:36 by weiyang          #+#    #+#             */
/*   Updated: 2026/02/17 20:48:15 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：tile_blocks（static）
** 作用：判断某一个地图格字符是否阻挡玩家（墙/空格/关门）
** 参数：
**   game：总结构体
**   x, y：地图格坐标
** 返回：
**   true  = 阻挡
**   false = 不阻挡
** 调用：
**   blocked_with_buffer()
*/
static bool	tile_blocks(t_game *game, int x, int y)
{
	char	c;

	if (x < 0 || y < 0 || x >= game->map_w || y >= game->map_h)
		return (true);
	c = game->map[y][x];
	if (c == '1' || c == ' ')
		return (true);
#ifdef BONUS
	if (c == 'D')
	{
		/* door_state = 0 关门挡路；=1 开门不挡 */
		if (game->door_state && game->door_state[y][x] == 0)
			return (true);
	}
#endif
	return (false);
}

/*
** 函数：blocked_with_buffer（static）
** 作用：判断坐标 (nx, ny) 是否“碰墙”（考虑缓冲距离 WALL_BUFFER）
** 参数：
**   game：总结构体
**   nx, ny：玩家尝试移动到的新坐标
** 返回：
**   true  = 会撞墙/越界/走到空白/撞到关门
**   false = 可以通过
** 调用：
**   try_move_axis()
*/
static bool	blocked_with_buffer(t_game *game, float nx, float ny)
{
	int	x0;
	int	x1;
	int	y0;
	int	y1;

	if (nx < 0.0f || ny < 0.0f)
		return (true);
	if (nx >= (float)game->map_w || ny >= (float)game->map_h)
		return (true);
	x0 = (int)(nx - WALL_BUFFER);
	x1 = (int)(nx + WALL_BUFFER);
	y0 = (int)(ny - WALL_BUFFER);
	y1 = (int)(ny + WALL_BUFFER);
	if (tile_blocks(game, x0, y0))
		return (true);
	if (tile_blocks(game, x1, y0))
		return (true);
	if (tile_blocks(game, x0, y1))
		return (true);
	if (tile_blocks(game, x1, y1))
		return (true);
	return (false);
}

/*
** 函数：try_move_axis（static）
** 作用：把移动拆成“先走 x 再走 y”，减少卡墙/斜角抖动
** 参数：
**   game：总结构体
**   dx, dy：本帧期望移动的位移量
*/
static void	try_move_axis(t_game *game, float dx, float dy)
{
	float	nx;
	float	ny;

	nx = game->player.x + dx;
	ny = game->player.y;
	if (!blocked_with_buffer(game, nx, ny))
		game->player.x = nx;
	nx = game->player.x;
	ny = game->player.y + dy;
	if (!blocked_with_buffer(game, nx, ny))
		game->player.y = ny;
}

/*
** 函数：get_move_offset
** 作用：计算玩家在当前按键状态下，相对于当前角度的 $x$ 和 $y$ 轴位移增量。
** 参数：
** game：总结构体，用于获取角度、移动速度和按键状态。
** dx：指向 float 的指针，用于写回计算出的 $x$ 轴总位移。
** dy：指向 float 的指针，用于写回计算出的 $y$ 轴总位移。
** 调用：被 update_player 调用。
** 主要逻辑：
** 1. 将当前的移动向量分解为 $ca$ ($cos$ 分量) 和 $sa$ ($sin$ 分量)。
** 2. 根据 WASD 按键状态，利用垂直向量原理叠加“前后”和“左右”的位移。
*/
static void	get_move_offset(t_game *game, float *dx, float *dy)
{
	float	ca;
	float	sa;

	ca = cosf(game->player.angle) * game->player.move_speed;
	sa = sinf(game->player.angle) * game->player.move_speed;
	if (game->player.key_up)
	{
		*dx += ca;
		*dy += sa;
	}
	if (game->player.key_down)
	{
		*dx -= ca;
		*dy -= sa;
	}
	if (game->player.key_left)
	{
		*dx -= sa;
		*dy += ca;
	}
	if (game->player.key_right)
	{
		*dx += sa;
		*dy -= ca;
	}
}

/*
** 函数：update_player
** 作用：每一帧更新玩家的状态，包括旋转角度的计算和最终位置的位移执行。
** 参数：
** game：总结构体指针。
** 调用：主循环 Hook 函数调用；内部调用 get_move_offset 和 try_move_axis。
** 主要逻辑：
** 1. 检测旋转按键并更新 angle，确保角度始终落在 $[0, 2\pi)$ 范围内。
** 2. 重置并调用辅助函数计算本帧期望的位移量 $dx$ 和 $dy$。
** 3. 若位移不为零，则调用轴拆分移动函数（处理碰撞检测）。
*/
void	update_player(t_game *game)
{
	float	dx;
	float	dy;

	if (game->player.key_rot_l)
		game->player.angle -= game->player.rot_speed;
	if (game->player.key_rot_r)
		game->player.angle += game->player.rot_speed;
	while (game->player.angle >= 2.0f * PI)
		game->player.angle -= 2.0f * PI;
	while (game->player.angle < 0.0f)
		game->player.angle += 2.0f * PI;
	dx = 0;
	dy = 0;
	get_move_offset(game, &dx, &dy);
	if (dx != 0.0f || dy != 0.0f)
		try_move_axis(game, dx, dy);
}
