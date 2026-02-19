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

    // 1. 统计并分配精灵列表内存
    count_sprites(game); 
    if (game->sprs.num > 0)
    {
        game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
        collect_sprites(game); // 提取 T/B/C/M 的坐标并抹除字符
    }

    // 2. 初始化门的数据
    init_doors(game); 

    // ❌ 删掉这里的 init_sprite_texture(game);
}
