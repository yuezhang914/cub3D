/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:56 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:43:08 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：build_map_array（static）
** 作用：把 .cub 里 map 区域的每一行，复制到 game->map（做成“矩形地图”）
** 参数：
**   game：写入 game->map（char**），并使用 game->map_h / game->map_w
**   lines：整个 .cub 行数组
**   start：map 第一行 index
** 主要逻辑：
**   1) 为 map 指针数组分配 (map_h + 1) 个 char*，最后一项放 NULL
**   2) 每一行分配 (map_w + 1) 个 char（最后 '\0'）
**   3) 把原始行复制进来：
**        - 遇到 '\t'（tab）强行转换成 ' '（空格）
**   4) 如果该行长度 < map_w，用空格补齐到统一宽度（形成规则矩形）
** 这样做的目的：
**   后面 scan_map 访问上下左右时不容易越界，并且空出来的部分都用 ' ' 表示“外部/无效”
** 调用者：
**   parse_map()
*/
static void	build_map_array(t_game *game, char **lines, int start)
{
	int	i;
	int	j;

	game->map = track_malloc(game, (game->map_h + 1) * sizeof(char *));
	i = -1;
	while (++i < game->map_h)
	{
		game->map[i] = track_malloc(game, (game->map_w + 1) * sizeof(char));
		j = 0;
		while (lines[start + i][j])
		{
			game->map[i][j] = lines[start + i][j];
			/* tab 统一当作空格处理 */
			if (lines[start + i][j] == '\t')
				game->map[i][j] = ' ';
			j++;
		}
		/* 不足宽度的部分，用空格补齐 */
		while (j < game->map_w)
			game->map[i][j++] = ' ';
		game->map[i][j] = '\0';
	}
	game->map[i] = NULL;
}

#ifdef BONUS

/*
** 函数名：parse_map（BONUS 版本）
** 作用：解析地图区并做 bonus 的额外初始化
** 参数：
**   game：最终得到 game->map / 玩家出生点等
** 主要逻辑：
**   1) find_map_start：找地图起点
**   2) check_map_is_last：保证地图后面没乱七八糟内容
**   3) set_map_dimensions：计算 map_h/map_w
**   4) build_map_array：复制成矩形二维数组
**   5) scan_map：扫描校验合法字符、封闭性、提取玩家出生点等
**   6) handle_bonus_setup：bonus 专用初始化（例如门/道具/敌人等）
** 调用者：
**   module_parse()
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
	scan_map(game);
	handle_bonus_setup(game);
}

#else

/*
** 函数名：parse_map（非 BONUS 版本）
** 作用：只做基础地图解析与校验
** 参数/逻辑与 BONUS 基本一致，只是没有 handle_bonus_setup
** 调用者：
**   module_parse()
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
	scan_map(game);
}
#endif
