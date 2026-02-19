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
** 函数：update_player
** 作用：根据按键状态更新玩家角度与位置（必做版）
** 参数：
**   game：总结构体（读取 game->player.key_*，写回 player.x/y/angle）
*/
void	update_player(t_game *game)
{
	float	dx;
	float	dy;
	float	ca;
	float	sa;

	/* ===== 1) 旋转（用你结构体里真实存在的 key_rot_l/key_rot_r + rot_speed） ===== */
	if (game->player.key_rot_l)
		game->player.angle -= game->player.rot_speed;
	if (game->player.key_rot_r)
		game->player.angle += game->player.rot_speed;
	/* 角度归一化到 [0, 2PI) */
	while (game->player.angle >= 2.0f * PI)
		game->player.angle -= 2.0f * PI;
	while (game->player.angle < 0.0f)
		game->player.angle += 2.0f * PI;
	/* ===== 2) 位移（前后 = 朝向方向；左右 = 朝向垂直方向） ===== */
	dx = 0.0f;
	dy = 0.0f;
	ca = cosf(game->player.angle);
	sa = sinf(game->player.angle);
	if (game->player.key_up)
	{
		dx += ca * game->player.move_speed;
		dy += sa * game->player.move_speed;
	}
	if (game->player.key_down)
	{
		dx -= ca * game->player.move_speed;
		dy -= sa * game->player.move_speed;
	}
	if (game->player.key_left)
	{
		dx += -sa * game->player.move_speed;
		dy += ca * game->player.move_speed;
	}
	if (game->player.key_right)
	{
		dx += sa * game->player.move_speed;
		dy += -ca * game->player.move_speed;
	}
	/* ===== 3) 碰撞处理（拆轴移动） ===== */
	if (dx != 0.0f || dy != 0.0f)
		try_move_axis(game, dx, dy);
}
