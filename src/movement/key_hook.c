/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
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



