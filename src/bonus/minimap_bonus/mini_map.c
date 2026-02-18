/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/18 01:25:47 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void put_mini_pixel(t_game *game, int x, int y, int color)
{
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return;
	put_pixel(x + 20, y + 20, color, game);
}

static void draw_mini_player(t_game *game)
{
	int center_x = MINI_WIDTH / 2;
	int center_y = MINI_HEIGHT / 2;
	int i;

	for (int y = -2; y <= 2; y++)
	{
		for (int x = -2; x <= 2; x++)
			put_mini_pixel(game, center_x + x, center_y + y, 0xFF0000);
	}
	for (i = 0; i < 12; i++)
	{
		int line_x = center_x + (int)(cos(game->player.angle) * i);
		int line_y = center_y + (int)(sin(game->player.angle) * i);
		put_mini_pixel(game, line_x, line_y, 0x00FF00);
	}
}

/**
 * 根据小地图像素坐标计算其在地图中对应的颜色
 */
static int get_mini_color(t_game *game, int x, int y)
{
	float map_x;
	float map_y;

	map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
	map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;
	if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
		return (0x1A1A1A);
	if (game->map[(int)map_y][(int)map_x] == '1')
		return (0xFFFFFF);
	return (0x333333);
}

/**
 * 核心渲染函数：以玩家为中心的动态滚动小地图 (符合 Norm 规范)
 */
void render_minimap(t_game *game)
{
	int x, y;

	// 1. 背景 + 墙
	y = 0;
	while (y < MINI_HEIGHT)
	{
		x = 0;
		while (x < MINI_WIDTH)
		{
			put_mini_pixel(game, x, y, get_mini_color(game, x, y));
			x++;
		}
		y++;
	}

	// 2. 玩家
	draw_mini_player(game);

	// 3. 绘制精灵（Bonus）
	#ifdef BONUS
	for (int i = 0; i < game->sprs.num; i++)
	{
		float dx = game->sprs.list[i].x - game->player.x;
		float dy = game->sprs.list[i].y - game->player.y;
		int sx = (int)(MINI_WIDTH / 2 + dx * game->pix_per_unit);
		int sy = (int)(MINI_HEIGHT / 2 + dy * game->pix_per_unit);
		// 小方块表示精灵
		for (int dy2 = -1; dy2 <= 1; dy2++)
			for (int dx2 = -1; dx2 <= 1; dx2++)
				put_mini_pixel(game, sx + dx2, sy + dy2, 0x00FF00);
	}
	#endif
}
