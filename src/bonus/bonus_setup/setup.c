/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:21 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 03:18:57 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void count_sprites(t_game *game)
{
    int i;
    int j;

    if (!game || !game->map)
        return;
    game->sprs.num = 0; // 初始化计数器
    i = -1;
    while (game->map[++i])
    {
        j = -1;
        while (game->map[i][++j])
        {
            /* 统计所有定义的精灵字符 */
            if (ft_strchr("TBCM", game->map[i][j]))
                game->sprs.num++;
        }
    }
}

/**
 * @brief Bonus 元素的统一处理函数
 * 1. 检查是否存在 Bonus 元素
 * 2. 如果有精灵 'C'，提取它们并抹除地图字符
 * 3. 如果有门 'D'，可以初始化门的状态数组
 */
void handle_bonus_setup(t_game *game)
{
	if (!game || !game->map)
		return;

	/* ** 1. 首先统计地图上所有精灵的总数
	** 这样才能为 game->sprs.list 分配正确的内存空间
	*/
	count_sprites(game);
	if (game->sprs.num > 0)
	{
		game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
		/* 2. 收集坐标、设置 type (T/B/C/M)、并从地图上抹除字符 */
		collect_sprites(game);
	}

	/* 3. 初始化门的数据结构 (记录坐标并分配 door_state 内存) */
	/* 注意：'D' 不要从地图上抹除，DDA 渲染时需要它 */
	init_doors(game);
}
