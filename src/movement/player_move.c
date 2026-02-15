/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:18:10 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:18:12 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 处理玩家旋转逻辑并更新角度
 */
static void	handle_rotation(t_player *player)
{
	if (player->left_rotate)
		player->angle -= player->rotate_speed;
	if (player->right_rotate)
		player->angle += player->rotate_speed;
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	else if (player->angle < 0)
		player->angle += 2 * PI;
}

/**
 * 根据按键状态计算位移向量
 */
static void	calculate_move(t_player *p, float *move_x, float *move_y)
{
	float	ca;
	float	sa;

	ca = cos(p->angle);
	sa = sin(p->angle);
	if (p->key_up)
	{
		*move_x += ca * p->move_speed;
		*move_y += sa * p->move_speed;
	}
	if (p->key_down)
	{
		*move_x -= ca * p->move_speed;
		*move_y -= sa * p->move_speed;
	}
	if (p->key_left)
	{
		*move_x += sa * p->move_speed;
		*move_y -= ca * p->move_speed;
	}
	if (p->key_right)
	{
		*move_x -= sa * p->move_speed;
		*move_y += ca * p->move_speed;
	}
}

/**
 * 碰撞检测函数 (Collision Detection)
 * 作用：判断玩家预移动到的浮点坐标 (px, py) 是否有效。通过将坐标转换为地图索引，
 * 检查其是否超出地图边界、是否触碰墙壁 ('1') 或进入非游戏区域 (' ')。
 * 返回值：若坐标不可通行（碰撞或越界）返回 true，否则返回 false。
 */
static bool touch(float px, float py, t_game *game)
{
	int x;
	int y;

	if (px < 0 || py < 0)
		return (true);
	x = (int)px;
	y = (int)py;
	if (y >= game->map_h || x >= game->map_w)
		return (true);
	if (game->map[y][x] == '1' || game->map[y][x] == ' ')
		return (true);
	return (false);
}

/**
 * 执行带有缓冲区的碰撞检测并应用最终坐标
 */
static void	apply_movement(t_player *p, t_game *g, float mx, float my)
{
	float	bx;
	float	by;

	bx = (mx > 0) ? 0.1f : -0.1f;
	by = (my > 0) ? 0.1f : -0.1f;
	if (!touch(p->x + mx + (mx != 0 ? bx : 0), p->y, g))
		p->x += mx;
	if (!touch(p->x, p->y + my + (my != 0 ? by : 0), g))
		p->y += my;
	p->map_x = (int)p->x;
	p->map_y = (int)p->y;
}

/**
 * 玩家移动逻辑执行器
 * 作用：依次处理旋转、位移计算、以及最终的碰撞应用。
 */
void	move_player(t_player *player, t_game *game)
{
	float	move_x;
	float	move_y;

	move_x = 0;
	move_y = 0;
	handle_rotation(player);
	calculate_move(player, &move_x, &move_y);
	apply_movement(player, game, move_x, move_y);
}
