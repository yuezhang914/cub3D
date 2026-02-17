/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:36 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：try_move_axis
** 作用：只尝试移动一个轴（x 或 y），并做“墙缓冲碰撞”。
** 参数：
**   game：总结构体
**   axis：'x' 或 'y'
**   delta：这一轴要移动的距离
** 用在哪：
**   move_by() 内部调用两次：一次移动 x，一次移动 y。
*/
void	try_move_axis(t_game *game, char axis, float delta)
{
	float	nx;
	float	ny;

	nx = game->player.x;
	ny = game->player.y;
	if (axis == 'x')
		nx += delta;
	if (axis == 'y')
		ny += delta;
	if (axis != 'x' && axis != 'y')
		graceful_exit(game, 1, __func__, "Wrong axis.");
	if (blocked_with_buffer(game, nx, ny))
		return ;
	if (axis == 'x')
		game->player.x += delta;
	if (axis == 'y')
		game->player.y += delta;
}

/*
** 函数：move_by（static）
** 作用：把一次“二维移动(dx,dy)”拆成两次“单轴移动”，更容易避免卡墙。
** 参数：
**   game：总结构体
**   dx, dy：本帧想移动的 x/y 距离
** 用在哪：
**   update_player() 中用于 W/S/A/D 的移动。
*/
static void	move_by(t_game *game, float dx, float dy)
{
	try_move_axis(game, 'x', dx);
	try_move_axis(game, 'y', dy);
}

/*
** 函数：blocked_with_buffer（static）
** 作用：用 4 个角点做碰撞检测（给玩家留一个 WALL_BUFFER 缓冲，不会贴墙穿模）。
** 参数：
**   game：总结构体
**   nx, ny：假设移动到的新位置
** 返回：
**   true：会撞墙/不能走
**   false：可以走
** 用在哪：
**   try_move_axis() 内部。
*/
static bool	blocked_with_buffer(t_game *game, float nx, float ny)
{
	if (game->map[(int)(ny + WALL_BUFFER)][(int)(nx + WALL_BUFFER)] != '0')
		return (true);
	if (game->map[(int)(ny - WALL_BUFFER)][(int)(nx - WALL_BUFFER)] != '0')
		return (true);
	if (game->map[(int)(ny + WALL_BUFFER)][(int)(nx - WALL_BUFFER)] != '0')
		return (true);
	if (game->map[(int)(ny - WALL_BUFFER)][(int)(nx + WALL_BUFFER)] != '0')
		return (true);
	return (false);
}

/*
** 函数：update_player
** 作用：根据按键状态更新玩家角度和位置（真正的移动在这里做）。
** 参数：
**   game：总结构体（读 player 的按键状态，写 player 的位置/角度）
** 用在哪：
**   game_step() 每帧调用一次。
*/
void	update_player(t_game *game)
{
	if (game->player.left_rotate)
		game->player.angle += game->player.rotate_speed;
	if (game->player.right_rotate)
		game->player.angle -= game->player.rotate_speed;
	if (game->player.angle > 2.0f * PI)
		game->player.angle -= 2.0f * PI;
	if (game->player.angle < 0.0f)
		game->player.angle += 2.0f * PI;
	if (game->player.key_up)
		move_by(game, cos(game->player.angle) * game->player.move_speed,
			-sin(game->player.angle) * game->player.move_speed);
	if (game->player.key_down)
		move_by(game, -cos(game->player.angle) * game->player.move_speed,
			sin(game->player.angle) * game->player.move_speed);
	if (game->player.key_right)
		move_by(game, sin(game->player.angle) * game->player.move_speed,
			cos(game->player.angle) * game->player.move_speed);
	if (game->player.key_left)
		move_by(game, -sin(game->player.angle) * game->player.move_speed,
			-cos(game->player.angle) * game->player.move_speed);
}
