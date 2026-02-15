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
 * 初始化玩家结构体
 * 将玩家放在屏幕中央，并重置所有按键状态。
 */
void init_player(t_player *player)
{
	// 重置按键状态
	player->key_up = false;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->left_rotate = false;
	player->right_rotate = false;

	// 建议加上移动和旋转的速度配置
	// 因为现在用的是地图单位（1.0 是一个格子），所以数值要小
	player->move_speed = 0.05f;
	player->rotate_speed = 0.04f;
}

/**
 * 键盘按下钩子 (Key Press Hook)
 * 作用：当检测到按键按下，只负责将对应的标志位设为 true。
 * 这样做可以避免操作系统默认的“按键重复延迟”导致的卡顿现象。
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
	// 如果需要按 ESC 退出，也可以加在这里
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
 * 作用：当手指离开按键，立刻将标志位设为 false，停止该方向的移动。
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

	// 补上旋转键的释放
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;

	return 0;
}

/**
 * 碰撞检测函数：判断坐标 (px, py) 是否位于墙壁内
 */
bool touch(float px, float py, t_game *game)
{
	// 1. 基础边界检查：防止 px 或 py 变成负数
	if (px < 0 || py < 0)
		return (true);

	// 2. 转换坐标：直接取整就是地图数组索引
	// 不再需要除以 BLOCK，因为现在的单位就是 1.0
	int x = (int)px;
	int y = (int)py;

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
    float move_x = 0;
    float move_y = 0;

    // 1. 处理旋转 (这部分你已经成功了)
    if (player->left_rotate) player->angle -= player->rotate_speed;
    if (player->right_rotate) player->angle += player->rotate_speed;

    // 2. 核心移动计算
    float cos_a = cos(player->angle);
    float sin_a = sin(player->angle);

    // 计算这一帧总的位移意图
    if (player->key_up) {
        move_x += cos_a * player->move_speed;
        move_y += sin_a * player->move_speed;
    }
    if (player->key_down) {
        move_x -= cos_a * player->move_speed;
        move_y -= sin_a * player->move_speed;
    }
    if (player->key_left) {
        move_x += sin_a * player->move_speed;
        move_y -= cos_a * player->move_speed;
    }
    if (player->key_right) {
        move_x -= sin_a * player->move_speed;
        move_y += cos_a * player->move_speed;
    }

    // 3. 碰撞检测与应用 (加上小的 buffer 防止贴墙死机)
    float buf_x = (move_x > 0) ? 0.1 : -0.1;
    float buf_y = (move_y > 0) ? 0.1 : -0.1;

    if (!touch(player->x + move_x + (move_x != 0 ? buf_x : 0), player->y, game))
        player->x += move_x;
    if (!touch(player->x, player->y + move_y + (move_y != 0 ? buf_y : 0), game))
        player->y += move_y;
}