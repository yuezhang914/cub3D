/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:00:39 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 辅助函数：将像素画到小地图区域（带 20 像素的边距偏移）
 */
static void put_mini_pixel(t_game *game, int x, int y, int color)
{
	// 确保不超出小地图定义的 200x200 区域
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return;
	// 这里的 20 是在主窗口左上角的起始偏移量
	put_pixel(x + 20, y + 20, color, game);
}

/**
 * 绘制玩家标识和方向线
 */
static void draw_mini_player(t_game *game)
{
	int center_x = MINI_WIDTH / 2;
	int center_y = MINI_HEIGHT / 2;
	int i;

	// 1. 绘制一个 5x5 的红色像素块代表玩家
	for (int y = -2; y <= 2; y++)
	{
		for (int x = -2; x <= 2; x++)
			put_mini_pixel(game, center_x + x, center_y + y, 0xFF0000);
	}
	// 2. 绘制一条绿色的方向线（长度为 12 像素）
	for (i = 0; i < 12; i++)
	{
		int line_x = center_x + (int)(cos(game->player.angle) * i);
		int line_y = center_y + (int)(sin(game->player.angle) * i);
		put_mini_pixel(game, line_x, line_y, 0x00FF00);
	}
}

/**
 * 核心渲染函数：以玩家为中心的动态滚动小地图
 */
void render_minimap(t_game *game)
{
	int x;
	int y;
	float map_x;
	float map_y;
	int color;

	y = 0;
	while (y < MINI_HEIGHT)
	{
		x = 0;
		while (x < MINI_WIDTH)
		{
			// 利用你定义的 pix_per_unit 进行坐标反推
			// 原理：玩家坐标 + (当前像素与中心的距离 / 比例尺)
			map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
			map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;

			// 1. 安全检查：防止地图数组越界（Segfault 杀手）
			if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
				color = 0x1A1A1A; // 地图外的虚无（深灰）
			// 2. 墙壁检测
			else if (game->map[(int)map_y][(int)map_x] == '1')
				color = 0xFFFFFF; // 墙壁（白色）
			// 3. 空地
			else
				color = 0x333333; // 地面（浅灰）

			put_mini_pixel(game, x, y, color);
			x++;
		}
		y++;
	}
	// 在背景画完后，最后覆盖玩家位置
	draw_mini_player(game);
}
