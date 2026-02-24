/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_sprites_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:41 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:35:44 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	fill_sprite_data(t_game *game, int i, int j, int *count)
{
	char	c;

	/* 1. 获取当前网格坐标 (i, j) 处的字符类型 */
	c = game->map[i][j];

	/* 2. 设置精灵的世界坐标 */
	/* 加上 0.5f 是为了将精灵放置在方格的正中心，而不是左上角角点 */
	/* j 对应地图的 X 轴，i 对应地图的 Y 轴 */
	game->sprs.list[*count].x = j + 0.5f;
	game->sprs.list[*count].y = i + 0.5f;

	/* 3. 分类标识：根据字符为精灵分配类型 */
	/* 这些类型 (SPR_TREE 等) 通常是枚举值，用于后续关联不同的纹理图片 */
	if (c == 'T')
		game->sprs.list[*count].type = SPR_TREE;
	else if (c == 'B')
		game->sprs.list[*count].type = SPR_BARREL;
	else if (c == 'C')
		game->sprs.list[*count].type = SPR_TORCH;
	else if (c == 'M')
		game->sprs.list[*count].type = SPR_MONSTER;

	/* 4. 清理地图 */
	/* 将该位置重置为 '0' (空地)，这样 DDA 射线就能穿过精灵所在的位置 */
	/* 精灵的碰撞和渲染将由专门的精灵引擎独立处理 */
	game->map[i][j] = '0';

	/* 5. 递增计数器，准备填充下一个精灵 */
	(*count)++;
}

void	collect_sprites(t_game *game)
{
	int	i;
	int	j;
	int	count;

	/* 1. 预扫描：统计地图中精灵的总数 */
	/* 这一步通常会遍历地图统计 'T','B','C','M' 出现的次数，存入 game->sprs.num */
	count_sprites(game);

	/* 2. 空校验：如果地图上没有精灵，直接返回 */
	if (game->sprs.num == 0)
		return ;

	/* 3. 内存分配：根据精灵数量动态分配列表空间 */
	/* track_malloc 是一个带垃圾回收或错误检查的封装函数，确保分配 sizeof(t_sprite) * num 字节 */
	game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);

	/* 4. 二次扫描：提取数据 */
	count = 0;
	i = -1;
	/* 遍历地图的每一行 (Y轴) */
	while (game->map[++i])
	{
		j = -1;
		/* 遍历地图的每一列 (X轴) */
		while (game->map[i][++j])
		{
			/* 5. 识别标识符：如果当前格子是树(T)、木桶(B)、火炬(C)或怪物(M) */
			if (ft_strchr("TBCM", game->map[i][j]))
			{
				/* 6. 填充数据：将该位置的信息转存到 list[count] 中 */
				/* 该函数还会把地图上的字符抹除（变回 '0'），并递增 count */
				fill_sprite_data(game, i, j, &count);
			}
		}
	}
}
