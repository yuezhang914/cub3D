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
** 函数名：tile_blocks（static）
** 作用：判断地图某一个“格子 (x,y)”是否会阻挡玩家（走不进去）
**
** 参数：
**   game：提供地图 game->map、尺寸 map_w/map_h、以及 door_state
**   x,y：格子坐标（整数格）
**
** 返回：
**   true：这个格子会挡住（墙/地图外/关闭的门/空格）
**   false：不阻挡（可走地面，或打开的门）
**
** 主要逻辑：
**   1) 越界：当作阻挡（防止走到地图外）
**   2) 字符是 '1' 或 ' '：阻挡
**      - ' ' 在你们地图里通常代表“无效区域/地图外”，也必须挡
**   3) 字符是 'D'（门）：
**      - door_state[y][x]==0：门关着 → 阻挡
**      - door_state[y][x]==1：门开着 → 不阻挡
**
** 调用者：
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
	if (c == 'D')
	{
		if (game->door_state && game->door_state[y][x] == 0)
			return (true);
	}
	return (false);
}

/*
** 函数名：blocked_with_buffer（static）
** 作用：判断玩家如果移动到 (nx,ny) 是否会“撞墙”
**      这里不是把玩家当点，而是给玩家一个“身体半径”缓冲 WALL_BUFFER
**
** 参数：
**   game：提供地图与 tile_blocks
**   nx,ny：玩家尝试移动到的新坐标（浮点数）
**
** 返回：
**   true：会撞（不能移动）
**   false：不撞（可以移动）
**
** 主要逻辑（核心：检查四个角）：
**   1) 先检查 nx,ny 是否在地图范围内（浮点范围）
**   2) 计算四个角落点所在的格子：
**      x0 = (int)(nx - WALL_BUFFER)
**      x1 = (int)(nx + WALL_BUFFER)
**      y0 = (int)(ny - WALL_BUFFER)
**      y1 = (int)(ny + WALL_BUFFER)
**   3) 只要四个角中任意一个落在阻挡格子里，就算会撞
**
** 为什么这样做（初中版）：
**   玩家不是“一个像素点”，而是有体积的人。
**   如果只检查中心点，中心能过但身体会穿进墙里。
**   所以用 buffer 检查“身体四角”更真实。
**
** 调用者：
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
** 函数名：try_move_axis（static）
** 作用：按“分轴移动”策略尝试移动玩家：先只动 x，再只动 y
**
** 参数：
**   game：读写 game->player.x/y，并调用 blocked_with_buffer
**   dx,dy：本帧计算出的位移（可能同时非 0，代表斜着走）
**
** 主要逻辑：
**   1) 先尝试 x 方向移动：nx=x+dx, ny=y
**      - 不撞墙则更新 x
**   2) 再尝试 y 方向移动：nx=更新后的 x, ny=y+dy
**      - 不撞墙则更新 y
**
** 为什么要分轴（初中版）：
**   斜着走时容易被墙角卡住。
**   分开走能让你“贴着墙滑过去”，手感更好。
**
** 调用者：
**   update_player()
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
** 函数名：get_move_offset（static）
** 作用：根据当前按键状态（W/S/A/D）计算本帧位移 (dx,dy)
**
** 参数：
**   game：提供 player.angle（朝向角）与 move_speed（速度），以及 key 状态
**   dx,dy：输出参数，会在原值基础上累加（所以调用前应置 0）
**
** 主要逻辑（角度→方向向量）：
**   ca = cos(angle) * speed
**   sa = sin(angle) * speed
**
**   - 前进(W)：dx += ca, dy += sa
**   - 后退(S)：dx -= ca, dy -= sa
**   - 左移(A)：dx -= sa, dy += ca   （相当于朝向向量左转 90°）
**   - 右移(D)：dx += sa, dy -= ca   （相当于朝向向量右转 90°）
**
** 抽象概念解释（初中版）：
**   cos/sin 就是把“朝向角度”拆成 x 方向分量和 y 方向分量，
**   所以你面朝哪边，就能算出往那边走每一步在 x/y 上各走多少。
**
** 调用者：
**   update_player()
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
** 函数名：update_player
** 作用：每帧更新玩家状态：先处理旋转，再处理移动（含碰撞）
**
** 参数：
**   game：读写 game->player 的 angle/x/y，并读取 key 状态
**
** 主要逻辑：
**   1) 旋转：
**      - key_rot_l：angle -= rot_speed
**      - key_rot_r：angle += rot_speed
**   2) 角度归一化到 [0, 2PI)：
**      - angle >= 2PI 就减 2PI
**      - angle < 0 就加 2PI
**      这样 angle 不会无限增大，减少浮点误差
**   3) 位移：
**      - dx=0, dy=0
**      - get_move_offset 计算 dx,dy
**      - 如果 dx/dy 非 0，用 try_move_axis 做分轴移动与碰撞检测
**
** 调用者：
**   game_step()（loop.c）
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
