/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:36 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 20:48:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：blocked_with_buffer（static）
** 作用：判断坐标 (nx, ny) 是否“碰墙”（考虑一个缓冲距离 WALL_BUFFER）
** 参数：
**   game：总结构体（用 game->map/map_w/map_h）
**   nx, ny：玩家尝试移动到的新坐标（float）
** 返回：
**   true  = 会撞墙/越界/走到空白区域
**   false = 可以通过
*/
static bool	blocked_with_buffer(t_game *game, float nx, float ny)
{
	int	x0;
	int	x1;
	int	y0;
	int	y1;

	/* 越界直接视为不可走 */
	if (nx < 0.0f || ny < 0.0f)
		return (true);
	if (nx >= (float)game->map_w || ny >= (float)game->map_h)
		return (true);
	/*
	** 用一个“方形缓冲盒”近似玩家体积：
	** 检查 (nx±buffer, ny±buffer) 所在地图格是否是墙 '1' 或空格 ' '
	*/
	x0 = (int)(nx - WALL_BUFFER);
	x1 = (int)(nx + WALL_BUFFER);
	y0 = (int)(ny - WALL_BUFFER);
	y1 = (int)(ny + WALL_BUFFER);
	if (x0 < 0 || y0 < 0 || x1 >= game->map_w || y1 >= game->map_h)
		return (true);
	if (game->map[y0][x0] == '1' || game->map[y0][x0] == ' ')
		return (true);
	if (game->map[y0][x1] == '1' || game->map[y0][x1] == ' ')
		return (true);
	if (game->map[y1][x0] == '1' || game->map[y1][x0] == ' ')
		return (true);
	if (game->map[y1][x1] == '1' || game->map[y1][x1] == ' ')
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
		game->player.angle += game->player.rot_speed;
	if (game->player.key_rot_r)
		game->player.angle -= game->player.rot_speed;
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
