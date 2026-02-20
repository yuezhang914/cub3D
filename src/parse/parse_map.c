/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:56 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 17:50:46 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "func.h"

/*
** 函数名：build_map_array（static）
** 作用：根据已算好的 game->map_h / game->map_w，把地图段复制成统一大小的二维数组 game->map。
**      关键点：原文件里每行长度不同，所以这里会把短行右侧用空格 ' ' 补齐到 map_w，
**      这样后续检查 “上下左右邻居” 时不会越界，也更容易判断“漏气墙”。
**
** 参数：
**   game  ：全局上下文（写入 game->map）
**   lines ：.cub 按行数组
**   start ：地图开始行下标
**
** 返回：
**   无；失败时 track_malloc 内部返回 NULL 会导致后续崩溃，所以一般配合 graceful_exit 或确保 track_malloc 不返回 NULL
**
** 用在哪里：
**   parse_map()：在 set_map_dimensions 之后调用，构建 game->map。
*/
static void	build_map_array(t_game *game, char **lines, int start)
{
	int	i;
	int	j;

	game->map = track_malloc(game, (game->map_h + 1) * sizeof(char *));
	i = 0;
	while (i < game->map_h)
	{
		game->map[i] = track_malloc(game, (game->map_w + 1) * sizeof(char));
		j = 0;
		while (lines[start + i][j])
		{
			game->map[i][j] = lines[start + i][j];
			j++;
		}
		while (j < game->map_w)
		{
			game->map[i][j] = ' ';
			j++;
		}
		game->map[i][j] = '\0';
		i++;
	}
	game->map[i] = NULL;
}


/*
** 函数名：parse_map
** 作用：地图解析总入口（只负责“调度步骤”，不写复杂逻辑）：
**      1) 找到地图开始行（find_map_start）
**      2) 检查地图必须是最后一段（check_map_is_last）
**      3) 统计地图尺寸 map_h/map_w（set_map_dimensions）
**      4) 构建统一宽度的二维 map 数组（build_map_array）
**      5) 扫描地图做封闭检查 + 抽取玩家出生点（scan_map）
**
** 参数：
**   game：全局上下文（最终会写入 game->map、game->map_h/map_w、game->player 初始状态）
**
** 返回：
**   无；任何错误直接 graceful_exit 退出
**
** 用在哪里：
**   parse 入口 module_parse() 中，在 parse_config() 之后调用。
*/
void	parse_map(t_game *game)
{
	int		start;
	char	**lines;

	lines = game->cubfile_lines;
	start = find_map_start(game, lines);
	check_map_is_last(game, lines, start);
	set_map_dimensions(game, lines, start);
	build_map_array(game, lines, start);
	scan_map(game); // 必做的合法性检查：玩家点、墙封闭
/* --- [Bonus 逻辑插入点] --- */
#ifdef BONUS
	handle_bonus_setup(game);
#endif
}
