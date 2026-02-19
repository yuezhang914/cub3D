/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bonus_elements.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:31:39 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 10:31:42 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief 扫描地图，检查是否存在精灵(S)或门(D)。
 * 这是一个快速扫描，用于决定后续是否分配精灵内存。
 */
bool    check_bonus_elements(t_game *game)
{
    int i;
    int j;

    i = 0;
    while (game->map && game->map[i])
    {
        j = 0;
        while (game->map[i][j])
        {
            if (ft_strchr("CDTMB", game->map[i][j]))
                return (true);
            j++;
        }
        i++;
    }
    return (false);
}
