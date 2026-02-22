/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:21 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:46:03 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：count_sprites
** 作用：扫描整张地图，统计有多少个“精灵字符”（T/B/C/M），并写入 game->sprs.num
**
** 参数：
**   game：游戏总结构体
**     - 读取：game->map（地图二维字符数组）
**     - 写入：game->sprs.num（精灵数量）
**
** 主要逻辑（逐格扫描）：
**   1) 如果 game 或 map 为空，直接 return（防止空指针崩溃）
**   2) num 清零
**   3) 双层循环遍历 map[i][j]
**   4) 如果 map[i][j] 属于 "TBCM"（用 ft_strchr 判断）：
**        num++
**
** 抽象概念解释（为什么要先 count）：
**   你们的精灵列表 sprs.list 是一个数组，
**   数组分配必须先知道有多少个元素，才能 malloc 正确大小。
**
** 调用者：
**   handle_bonus_setup()：在分配 sprs.list 之前必须先调用
*/
void	count_sprites(t_game *game)
{
	int	i;
	int	j;

	if (!game || !game->map)
		return ;
	game->sprs.num = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (ft_strchr("TBCM", game->map[i][j]))
				game->sprs.num++;
		}
	}
}

/*
** 函数名：handle_bonus_setup
** 作用：BONUS 初始化总入口：准备精灵列表 + 初始化门状态表
**
** 参数：
**   game：游戏总结构体
**     - 读取：game->map
**     - 写入：game->sprs.num / game->sprs.list / game->door_state（在 init_doors 里）
**
** 主要逻辑（按顺序非常关键）：
**   1) 空指针保护：game 或 map 为空直接 return
**   2) count_sprites(game)：
**      - 统计地图里 T/B/C/M 的总数量，写入 sprs.num
**   3) 如果 sprs.num > 0：
**      3.1) 分配 sprs.list 数组（长度 = sprs.num）
**           - 用 track_malloc：这样退出时会被 track_clean 统一释放
**      3.2) collect_sprites(game)：
**           - 真正扫描地图，把每个精灵填进 sprs.list（位置、类型等）
**           - 通常还会把 map 里的字符改成 '0'（地面），避免影响其他逻辑
**   4) init_doors(game)：
**      - 分配 door_state 二维数组（与地图同样大小）
**      - 初始化为 0（默认关门，或者默认状态）
**
** 调用者（典型位置）：
**   通常在 parse_map() 之后调用：
**   因为它需要 game->map 已经建立好、map_w/map_h 已经算好。
**   你项目里 parse_map.c 的 BONUS 分支就是这么用的：
**     scan_map(game);
**     handle_bonus_setup(game);
*/
void	handle_bonus_setup(t_game *game)
{
	if (!game || !game->map)
		return ;
	count_sprites(game);
	if (game->sprs.num > 0)
	{
		game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
		collect_sprites(game);
	}
	init_doors(game);
}
