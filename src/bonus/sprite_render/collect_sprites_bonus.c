/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_sprites_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:41 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:34:47 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：fill_sprite_data（static）
** 作用：当地图中发现一个 sprite 字符（T/B/C/M）时，把它转成 t_sprite 存入 sprs.list[count]
**
** 参数：
**   game：提供 map 和 sprs.list
**   i,j：地图格子坐标（i 行，j 列）
**   count：当前已经收集了多少个 sprite（作为数组下标），函数内部会 ++
**
** 主要逻辑：
**   1) c = map[i][j] 取出字符
**   2) 把 sprite 放到格子中心：
**      x = j + 0.5
**      y = i + 0.5
**      这样它不会贴在格子角落，更自然
**   3) 根据字符决定 sprite 类型：
**      T->SPR_TREE, B->SPR_BARREL, C->SPR_TORCH, M->SPR_MONSTER
**   4) 把地图该位置改成 '0'：
**      表示“地面”，避免 sprite 字符影响后续 raycasting/碰撞判断
**   5) (*count)++
**
** 调用者：
**   collect_sprites() 在扫描 map 时调用
*/
static void	fill_sprite_data(t_game *game, int i, int j, int *count)
{
	char	c;

	c = game->map[i][j];
	game->sprs.list[*count].x = j + 0.5f;
	game->sprs.list[*count].y = i + 0.5f;
	if (c == 'T')
		game->sprs.list[*count].type = SPR_TREE;
	else if (c == 'B')
		game->sprs.list[*count].type = SPR_BARREL;
	else if (c == 'C')
		game->sprs.list[*count].type = SPR_TORCH;
	else if (c == 'M')
		game->sprs.list[*count].type = SPR_MONSTER;
	game->map[i][j] = '0';
	(*count)++;
}

/*
** 函数名：collect_sprites
** 作用：扫描整张地图，把所有 sprite（T/B/C/M）收集到 game->sprs.list 数组里
**
** 参数：
**   game：提供 map；写入 sprs.num 和 sprs.list
**
** 主要逻辑：
**   1) count_sprites(game)：先统计有多少个 sprite，并写入 game->sprs.num
**      （count_sprites 实现在别处：通常也是扫描 map 统计 T/B/C/M）
**   2) 如果 sprs.num==0：直接 return
**   3) 分配 sprs.list：大小为 num 个 t_sprite
**   4) 再扫描 map：
**      - 遇到字符属于 "TBCM" 就调用 fill_sprite_data()
**
** 调用者：
**   bonus 初始化阶段：解析地图后（确保 map 已经建立）
*/
void	collect_sprites(t_game *game)
{
	int	i;
	int	j;
	int	count;

	count_sprites(game);
	if (game->sprs.num == 0)
		return ;
	game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
			if (ft_strchr("TBCM", game->map[i][j]))
				fill_sprite_data(game, i, j, &count);
	}
}
