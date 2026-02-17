/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:56 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 21:52:19 by yzhang2          ###   ########.fr       */
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
** 函数名：validate_open_walls（static）
** 作用：检查一个“可走格/出生点格”（0 或 N/E/S/W）是否被墙正确包围，防止“漏气”：
**      - 不能在边界（第一行/最后一行/第一列/最后一列）
**      - 上下左右不能是空格 ' '（空格代表地图外部或无效区域）
**
** 参数：
**   game：全局上下文（读取 game->map、game->map_h、game->map_w）
**   i   ：当前格子的行号
**   j   ：当前格子的列号
**
** 返回：
**   无；发现漏气直接 graceful_exit 退出
**
** 用在哪里：
**   scan_map() 扫描到字符属于 "NESfirst_word" 时调用。
*/
static void	validate_open_walls(t_game *game, int i, int j)
{
	if (i == 0 || j == 0 || i == game->map_h - 1 || j == game->map_w - 1)
		graceful_exit(game, 1, __func__, "Open wall found.");
	if (game->map[i - 1][j] == ' ' || game->map[i + 1][j] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
	if (game->map[i][j - 1] == ' ' || game->map[i][j + 1] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
}

/*
** 函数名：extract_player（static）
** 作用：处理出生点（N/E/S/W）：
**      1) 保证只有一个出生点（found 已经为 true 则报错）
**      2) 写入玩家初始坐标（放在格子中心：+0.5）
**      3) 根据 N/E/S/W 设置初始朝向角度（弧度）
**      4) 把地图里的 N/E/S/W 替换成 '0'，后续移动/碰撞只看 0/1/空格
**
** 参数：
**   game  ：全局上下文（写入 game->player.x/pos_y/angle，并修改 game->map[i][j]）
**   i, j  ：出生点所在的地图格子坐标
**   found ：外部传进来的标志（指针），表示是否已经找到过出生点
**
** 返回：
**   无；违规直接 graceful_exit 退出
**
** 用在哪里：
**   scan_map() 扫描到字符属于 "NESW" 时调用。
*/
static void	extract_player(t_game *game, int i, int j, bool *found)
{
	if (*found)
		graceful_exit(game, 1, __func__, "Multiple start position.");
	*found = true;
	game->player.x = (float)j + 0.5f;
	game->player.y = (float)i + 0.5f;
	game->player.map_x = j;
	game->player.map_y = i;
	if (game->map[i][j] == 'E')
		game->player.angle = PI * 0.0f;
	if (game->map[i][j] == 'N')
		game->player.angle = PI * 0.5f;
	if (game->map[i][j] == 'W')
		game->player.angle = PI * 1.0f;
	if (game->map[i][j] == 'S')
		game->player.angle = PI * 1.5f;
	game->map[i][j] = '0';
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
static void	scan_map(t_game *game)
{
	int		i;
	int		j;
	bool	found;

	found = false;
	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (ft_strchr("NESW", game->map[i][j]))
				extract_player(game, i, j, &found);
			if (game->map[i][j] == '0')
				validate_open_walls(game, i, j);
			j++;
		}
		i++;
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
	scan_map(game);
}
