/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:33:51 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:33:56 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_sprite_distances(t_game *game)
{
	int		i;
	float	dx;
	float	dy;

	i = 0;
	/* 遍历精灵列表中的每一个实例 */
	while (i < game->sprs.num)
	{
		/* 计算玩家与精灵在世界坐标系下的 X 和 Y 轴偏移量 */
		dx = game->player.x - game->sprs.list[i].x;
		dy = game->player.y - game->sprs.list[i].y;

		/* 核心优化：计算距离平方 (dx^2 + dy^2) */
		/* 我们只需要比较远近，而不需要精确的物理距离， */
		/* 因此省略了开方运算 (sqrt)，从而节省了 CPU 开销。 */
		game->sprs.list[i].dist = (dx * dx) + (dy * dy);
		i++;
	}
}

#ifdef BONUS
void	sort_sprites(t_game *game)
{
	int			i;
	int			j;
	t_sprite	temp;

	/* 1. 基础校验：如果没有精灵或只有一个，无需排序 */
	if (game->sprs.num < 2)
		return ;

	/* 2. 在排序前，先调用 update_sprite_distances 更新最新的距离数据 */
	update_sprite_distances(game);

	/* 3. 冒泡排序逻辑 */
	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			/* 4. 排序准则：如果前一个精灵比后一个精灵近，则交换它们 */
			/* 目标是让距离最大的（最远的）精灵排在数组的前面 */
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
#endif
