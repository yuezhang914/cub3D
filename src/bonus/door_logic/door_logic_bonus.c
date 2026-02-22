/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_logic_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:28:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:55:42 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：door_is_closed
** 作用：判断地图坐标 (x,y) 是否是一扇“关着的门”
**
** 参数：
**   game：需要 game->map / game->door_state / map_w/map_h
**   x,y：门格子的坐标（整数格子坐标）
**
** 返回：
**   1：是门，并且 door_state <= 0（关门）
**   0：其他情况（不是门/越界/数据不存在/开门）
**
** 主要逻辑：
**   1) game/door_state/map 为空 → 返回 0（无法判断）
**   2) 越界 → 返回 0（视为不是关门；注意：你们碰撞/射线一般自己也会做越界处理）
**   3) map[y][x] 不是 'D' → 返回 0
**   4) door_state[y][x] <= 0 → 关门 → 返回 1
**
** 调用者（典型）：
**   - movement 的碰撞检测（门关着算阻挡）
**   - 其他逻辑要判断门状态时
*/
int	door_is_closed(t_game *game, int x, int y)
{
	if (!game || !game->door_state || !game->map)
		return (0);
	if (x < 0 || y < 0 || x >= game->map_w || y >= game->map_h)
		return (0);
	if (game->map[y][x] != 'D')
		return (0);
	return (game->door_state[y][x] <= 0.0f);
}

/*
** 函数名：init_doors
** 作用：为整张地图分配 door_state 二维数组，并全部初始化为 0（关门）
**
** 参数：
**   game：需要 game->map_h / map_w / track_malloc / ft_bzero
**
** 主要逻辑：
**   1) 分配 door_state 指针数组：map_h 行
**   2) 每一行分配 map_w 个 unsigned char
**   3) ft_bzero 清零：每个格子的门状态默认 0
**
** 重要说明：
**   - 这里并不会检查 map[y][x] 是否是 'D'，而是整张表都分配并初始化
**   - 后续判断是否是门以 map[y][x]=='D' 为准
**
** 调用者（典型）：
**   - bonus setup 初始化阶段（解析 map 后）
*/
void	init_doors(t_game *game)
{
	int	y;

	if (!game || !game->map)
		return ;
	game->door_state = (unsigned char **)track_malloc(game,
			sizeof(unsigned char *) * game->map_h);
	y = 0;
	while (y < game->map_h)
	{
		game->door_state[y] = (unsigned char *)track_malloc(game,
				sizeof(unsigned char) * game->map_w);
		ft_bzero(game->door_state[y], (size_t)game->map_w);
		y++;
	}
}

/*
** 函数名：door_target_tile（static）
** 作用：计算“玩家面前 0.75 格”所在的地图格子坐标（作为交互目标格子）
**
** 参数：
**   game：提供 player.x/y 和 player.angle
**
** 返回：
**   t_int_xy pos：目标格子坐标（pos.x,pos.y）
**
** 主要逻辑（数学解释）：
**   1) (cos(angle), sin(angle)) 是玩家面朝方向的单位向量
**   2) 往前走 0.75 格：
**        rx = player.x + cos(angle)*0.75
**        ry = player.y + sin(angle)*0.75
**   3) 对 rx,ry 取 int 得到格子坐标
**
** 为什么是 0.75 而不是 1.0？
**   - 站在格子边界附近时，用 1.0 可能跳到更远一格，体验不稳定
**   - 0.75 更像“伸手够到面前那格”
**
** 调用者：
**   try_toggle_door()
*/
static t_int_xy	door_target_tile(t_game *game)
{
	t_int_xy	pos;
	float		rx;
	float		ry;

	rx = game->player.x + cosf(game->player.angle) * 0.75f;
	ry = game->player.y + sinf(game->player.angle) * 0.75f;
	pos.x = (int)rx;
	pos.y = (int)ry;
	return (pos);
}

/*
** 函数名：try_toggle_door
** 作用：尝试切换玩家面前门的开关状态（0 <-> 1）
**
** 参数：
**   game：提供 door_state、map、map_w/map_h、player
**
** 主要逻辑：
**   1) 必要数据为空则 return
**   2) p = door_target_tile：找到玩家面前那一格
**   3) p 越界则 return
**   4) map[p.y][p.x] 不是 'D' 则 return（面前不是门）
**   5) door_state[p.y][p.x] = !door_state[p.y][p.x]
**      - 0 变 1：开门
**      - 1 变 0：关门
**
** 调用者：
**   input.c（BONUS）：按下 E 键时调用
*/
void	try_toggle_door(t_game *game)
{
	t_int_xy	p;

	if (!game || !game->door_state)
		return ;
	p = door_target_tile(game);
	if (p.x < 0 || p.y < 0 || p.x >= game->map_w || p.y >= game->map_h)
		return ;
	if (game->map[p.y][p.x] != 'D')
		return ;
	game->door_state[p.y][p.x] = (unsigned char)(!game->door_state[p.y][p.x]);
}
