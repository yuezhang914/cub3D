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


    player->x = WIDTH / 2;
    player->y = HEIGHT / 2;
	player->map_x = (player->x) / 64;
	player->map_y = (player->y) / 64;
	player->angle= PI / 2;
    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;

	player->left_rotate = false;
	player->right_rotate = false;
	
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

    if (keycode == W) player->key_up = false;
    if (keycode == S) player->key_down = false;
    if (keycode == A) player->key_left = false;
    if (keycode == D) player->key_right = false;
	
    // 补上旋转键的释放
    if (keycode == LEFT) player->left_rotate = false;
    if (keycode == RIGHT) player->right_rotate = false;

    return 0;
}

/**
 * 玩家移动逻辑执行器 (Frame-based Movement)
 * 作用：由 mlx_loop_hook 每一帧调用。
 * 只要标志位为 true，玩家就会在这一帧移动 speed 个像素。
 */
void move_player(t_player *player, t_game *game)
{
    float speed = 3.0;
    float angle_speed = 0.03;
    float next_x = 0;
    float next_y = 0;

    // 1. 处理旋转
    if (player->left_rotate)  player->angle -= angle_speed;
    if (player->right_rotate) player->angle += angle_speed;
    
    // 角度标准化
    if (player->angle > 2 * PI) player->angle -= 2 * PI;
    if (player->angle < 0)      player->angle += 2 * PI;

    // 2. 计算这一帧“想去”的位置偏移量
    float cos_a = cos(player->angle);
    float sin_a = sin(player->angle);

    if (player->key_up) {
        next_x += cos_a * speed;
        next_y += sin_a * speed;
    }
    if (player->key_down) {
        next_x -= cos_a * speed;
        next_y -= sin_a * speed;
    }
    if (player->key_left) { // 垂直向左
        next_x += sin_a * speed;
        next_y -= cos_a * speed;
    }
    if (player->key_right) { // 垂直向右
        next_x -= sin_a * speed;
        next_y += cos_a * speed;
    }

    // 3. 预测性碰撞检测 (Predictive Collision)
    // 分开检测 X 和 Y，实现沿墙滑动
    if (!touch(player->x + next_x, player->y, game))
        player->x += next_x;
    if (!touch(player->x, player->y + next_y, game))
        player->y += next_y;
}

