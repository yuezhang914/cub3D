/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:01:09 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/09 20:01:20 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 键盘按下钩子 (Key Press Hook)
 * 作用：当检测到按键按下时，将玩家结构体中对应的布尔标志位设为 true。
 * 这种基于状态位的方法可以绕过操作系统默认的按键延迟，实现多个按键（如前进+旋转）同时响应的丝滑操作。
 */
int key_press(int keycode, void *param)
{
	t_player *player = (t_player *)param;

	if (keycode == W)
		player->key_up = true;
	else if (keycode == S)
		player->key_down = true;
	else if (keycode == A)
		player->key_left = true;
	else if (keycode == D)
		player->key_right = true;
	else if (keycode == LEFT)
		player->left_rotate = true;
	else if (keycode == RIGHT)
		player->right_rotate = true;
	else if (keycode == ESC)
		exit(0);

	return 0;
}

/**
 * 键盘释放钩子 (Key Release Hook)
 * 作用：当检测到按键松开时，将玩家结构体中对应的标志位恢复为 false。
 * 这种机制确保了玩家只有在按键被持续按下时才会保持移动或旋转，一旦手指离开按键，相应的动作会立即停止。
 */
int key_release(int keycode, void *param)
{
	t_player *player = (t_player *)param;

	if (keycode == W)
		player->key_up = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;
	return 0;
}

/**
 * 碰撞检测函数 (Collision Detection)
 * 作用：判断玩家预移动到的浮点坐标 (px, py) 是否有效。通过将坐标转换为地图索引，
 * 检查其是否超出地图边界、是否触碰墙壁 ('1') 或进入非游戏区域 (' ')。
 * 返回值：若坐标不可通行（碰撞或越界）返回 true，否则返回 false。
 */
bool touch(float px, float py, t_game *game)
{
	int x;
	int y;

	// 1. 基础边界检查：防止 px 或 py 变成负数（地图坐标从 0 开始）
	if (px < 0 || py < 0)
		return (true);

	// 2. 坐标转换：将浮点数坐标直接取整，映射为地图数组的行(y)与列(x)索引
	// 不再需要除以 BLOCK，因为现在的单位就是 1.0
	x = (int)px;
	y = (int)py;
	// 3. 数组越界检查：使用解析出来的 map_h 和 map_w
	// 不要写死 10 或 15，因为地图大小是动态的
	if (y >= game->map_h || x >= game->map_w)
		return (true);

	// 4. 碰撞判定：如果格子里是墙 ('1') 或者是空格 (' ')，都不能走
	// (通常空格也被视为地图外部区域)
	if (game->map[y][x] == '1' || game->map[y][x] == ' ')
		return (true);
	return (false);
}

/**
 * 玩家移动逻辑执行器 (Frame-based Movement)
 * 作用：由 mlx_loop_hook 每一帧调用。
 * 只要标志位为 true，玩家就会在这一帧移动 speed 个像素。
 */
void move_player(t_player *player, t_game *game)
{
	float move_x;
	float move_y;
	float cos_a;
	float sin_a;
	float buf_x;
	float buf_y;

	move_x = 0;
	move_y = 0;

	// 1. 处理旋转
	if (player->left_rotate)
		player->angle -= player->rotate_speed;
	if (player->right_rotate)
		player->angle += player->rotate_speed;

	// 2. 核心移动计算
	cos_a = cos(player->angle);
	sin_a = sin(player->angle);

	// 计算这一帧总的位移意图
	if (player->key_up)
	{
		move_x += cos_a * player->move_speed;
		move_y += sin_a * player->move_speed;
	}
	if (player->key_down)
	{
		move_x -= cos_a * player->move_speed;
		move_y -= sin_a * player->move_speed;
	}
	if (player->key_left)
	{
		move_x += sin_a * player->move_speed;
		move_y -= cos_a * player->move_speed;
	}
	if (player->key_right)
	{
		move_x -= sin_a * player->move_speed;
		move_y += cos_a * player->move_speed;
	}
	// 3. 碰撞检测与应用 (加上小的 buffer 防止贴墙死机)
	buf_x = (move_x > 0) ? 0.1 : -0.1;
	buf_y = (move_y > 0) ? 0.1 : -0.1;
	if (!touch(player->x + move_x + (move_x != 0 ? buf_x : 0), player->y, game))
		player->x += move_x;
	if (!touch(player->x, player->y + move_y + (move_y != 0 ? buf_y : 0), game))
		player->y += move_y;
}