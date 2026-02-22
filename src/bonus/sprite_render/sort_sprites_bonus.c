/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:33:51 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:36:27 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：update_sprite_distances
** 作用：更新每个 sprite 到玩家的距离平方 dist（用于排序/遮挡）
**
** 参数：
**   game：提供玩家位置 player.x/y，以及 sprite 列表 sprs.list 和数量 sprs.num
**
** 主要逻辑：
**   dist = dx*dx + dy*dy（距离平方）
**   - 为什么不用 sqrt？
**     排序时比较大小即可，平方不会改变大小关系，省计算量
**
** 调用者：
**   sort_sprites()
**   render_sprites()（它在渲染前也会更新一次）
*/
void	update_sprite_distances(t_game *game)
{
	int		i;
	float	dx;
	float	dy;

	i = 0;
	while (i < game->sprs.num)
	{
		dx = game->player.x - game->sprs.list[i].x;
		dy = game->player.y - game->sprs.list[i].y;
		game->sprs.list[i].dist = (dx * dx) + (dy * dy);
		i++;
	}
}

#ifdef BONUS
/*
** 函数名：sort_sprites（BONUS）
** 作用：按距离从远到近排序 sprite 列表（dist 大的在前）
**
** 参数：
**   game：提供 sprs.list/sprs.num
**
** 主要逻辑：
**   1) 若 num<2：无需排序
**   2) update_sprite_distances()
**   3) 冒泡排序：
**      如果 list[j].dist < list[j+1].dist，就交换
**      → 结果 dist 大的排在前面（远的先画）
**
** 调用者：
**   loop.c bonus: render_sprites 前一般会先 sort（你 sprite_render_bonus.c 也会调用）
*/
void	sort_sprites(t_game *game)
{
	int			i;
	int			j;
	t_sprite	temp;

	if (game->sprs.num < 2)
		return ;
	update_sprite_distances(game);
	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
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
