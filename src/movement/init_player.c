/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:59:33 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 14:59:37 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 初始化玩家结构体
 * 作用：设置按键标志位为初始状态（未按下），并配置默认的移动与旋转速度。
 * 参数：player - 指向玩家数据结构 t_player 的指针。
 */
void init_player(t_player *player)
{
	player->key_up = false;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->left_rotate = false;
	player->right_rotate = false;
	player->move_speed = 0.05f;
	player->rotate_speed = 0.04f;
}
