/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:21 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 03:18:57 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

    if (check_bonus_elements(game))
    {
        init_doors(game);
        collect_sprites(game);   // ✅ 只收集坐标 + 抹除 'C'
        // init_sprite_texture(game);  // ❌ 这里不能加载（还没 mlx_init）
    }
}



