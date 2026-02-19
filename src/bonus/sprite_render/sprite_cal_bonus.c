/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_cal_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:33:41 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 11:33:43 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void init_sprite_texture(t_game *game)

{
	if (!game || !game->mlx)
		return;

	game->sprs.tex.path = "textures/tree.xpm";
	load_texture(game, &game->sprs.tex); // 里面会 remember_image + get_data_addr
}

void collect_sprites(t_game *game)
{
	int i;
	int j;
	int count;

	count = 0;
	// 1. 第一次遍历：数数
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
			if (game->map[i][j] == 'C')
				count++;
	}
	game->sprs.num = count;
	game->sprs.list = (t_sprite *)track_malloc(game, sizeof(t_sprite) * count);
	// 2. 第二次遍历：记录坐标并从地图抹除
	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (game->map[i][j] == 'C')
			{
				game->sprs.list[count].x = j + 0.5f;
				game->sprs.list[count].y = i + 0.5f;
				game->map[i][j] = '0'; // 变回空地，让射线穿过
				count++;
			}
		}
	}
	printf("After collect: %d sprites\n", game->sprs.num);
}

/**
 * @brief 计算每个精灵到玩家当前位置的距离平方
 * @param game 游戏主结构体
 */
void calculate_sprite_distance(t_game *game)
{
	int i;
	float px;
	float py;

	i = 0;
	px = game->player.x;
	py = game->player.y;
	while (i < game->sprs.num)
	{
		// 距离平方公式: (x1 - x2)^2 + (y1 - y2)^2
		game->sprs.list[i].dist = ((px - game->sprs.list[i].x) * (px - game->sprs.list[i].x) + (py - game->sprs.list[i].y) * (py - game->sprs.list[i].y));
		i++;
	}
}

/**
 * @brief 对精灵数组进行降序排序（从远到近）。
 * * 逻辑：Painter's Algorithm (画家算法)。
 * 我们必须先绘制最远的精灵，这样近处的精灵才能正确覆盖在其上。
 */
void sort_sprites(t_game *game)
{
	int i;
	int j;
	t_sprite temp;

	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			// 如果后一个精灵比前一个远，则交换位置
			if (game->sprs.list[j].dist < game->sprs.list[j + 1].dist)
			{
				temp = game->sprs.list[j];
				game->sprs.list[j] = game->sprs.list[j + 1];
				game->sprs.list[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
