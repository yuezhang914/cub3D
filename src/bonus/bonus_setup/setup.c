/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:21 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/18 18:13:23 by weiyang          ###   ########.fr       */
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
    printf("handle_bonus_setup called\n"); fflush(stdout);

    if (check_bonus_elements(game))
    {
        //printf("bonus elements found\n"); fflush(stdout);
        collect_sprites(game);
        //printf("collected %d sprites\n", game->sprs.num); fflush(stdout);
        init_sprite_texture(game);
		//如果是门， 则初始化门
		//。。。
    }
    else
    {
        printf("no bonus elements\n");
    }
}

