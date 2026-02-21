/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:33:51 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:33:56 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 计算每个精灵到玩家的距离平方
*/
void update_sprite_distances(t_game *game)
{
	int i;

	i = 0;
	while (i < game->sprs.num)
	{
		// 计算坐标差
		float dx = game->player.x - game->sprs.list[i].x;
		float dy = game->player.y - game->sprs.list[i].y;

		// 存储距离平方 (dx^2 + dy^2)
		game->sprs.list[i].dist = (dx * dx) + (dy * dy);
		i++;
	}
}

/*
** 执行排序 (画家算法：从远到近)
*/
void sort_sprites(t_game *game)
{
	int i;
	int j;
	t_sprite temp;

	if (game->sprs.num < 2)
		return;

	// 1. 先更新最新距离
	update_sprite_distances(game);

	// 2. 冒泡排序：将距离远的排在数组前面
	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			// 如果后一个比前一个更远，则交换（保证数组是从远到近）
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
