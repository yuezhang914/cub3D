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
** 函数名：scan_map（static）
** 作用：遍历 game->map 的每个字符，做两类检查/处理：
**      A) 对 "0 或 N/E/S/W" 做“漏气墙检查”（validate_open_walls）
**      B) 对 "N/E/S/W" 做“出生点抽取”（extract_player）
**      最后必须保证至少找到一个出生点，否则报错。
**
** 参数：
**   game：全局上下文（读写 player、读取 map）
**
** 返回：
**   无；发现任何违规直接 graceful_exit 退出
**
** 用在哪里：
**   parse_map()：在 build_map_array 之后调用。
*/
void scan_map(t_game *game)
{
	int i;
	int j;
	bool found;

	found = false;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			// ✅ 只有这四个标准方向字符能决定玩家起始位置
			if (ft_strchr("NESW", game->map[i][j]))
				extract_player(game, i, j, &found);
// ✅ 墙壁封闭性检查：所有通行区域都必须检查，防止射线穿出地图
// 包含 0, N, E, S, W 以及所有 Bonus 字符 T, M, B, C, D
// 包含 0, N, E, S, W
// Bonus 字符: T (Tree), B (Barrel), C (Torch), M (Monster), D (Door)
#ifdef BONUS
			// 注意：我们将 C 改为代表火炬，T 代表树，B 代表桶，M 代表怪物
			if (ft_strchr("0NESWTBCMD", game->map[i][j]))
				validate_open_walls(game, i, j);
#else
			if (ft_strchr("0NESW", game->map[i][j]))
				validate_open_walls(game, i, j);
#endif
		}
	}
	if (!found)
		graceful_exit(game, 1, __func__, "No start position found.");
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
