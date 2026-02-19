/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 17:49:30 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 18:07:11 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
static bool	is_allowed_tile(char c)
{
#ifdef BONUS
	return (ft_strchr(" 01NSEWCPRDdTBCM", c) != NULL);
#else
	return (ft_strchr(" 01NSEWTBCM", c) != NULL);
#endif
}


static bool	is_walkable_tile(char c)
{
#ifdef BONUS
	return (ft_strchr("0NESWCPRDdTBCM", c) != NULL);
#else
	return (ft_strchr("0NESWTBCM", c) != NULL);
#endif
}

void	scan_map(t_game *game)
{
	int		i;
	int		j;
	bool	found;
	char	c;

	found = false;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while ((c = game->map[i][++j]))
		{
			if (!is_allowed_tile(c))
				graceful_exit(game, 1, __func__, "Invalid map character.");
			if (ft_strchr("NESW", c))
				extract_player(game, i, j, &found);
			if (is_walkable_tile(c))
				validate_open_walls(game, i, j);
		}
	}
	if (!found)
		graceful_exit(game, 1, __func__, "No start position found.");
}
