/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 17:49:30 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:43:33 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 这里用宏定义“允许出现的字符集合”和“可走的字符集合”
** - ALLOWED_TILES：地图里允许出现的所有字符（包括空格）
** - WALKABLE_TILES：玩家能站/能走到的字符（只要是可走格，就必须被墙包围）
*/

#ifdef BONUS

# define ALLOWED_TILES " 01NSEWCPRDdTBCM"
# define WALKABLE_TILES "0NESWCPRDdTBCM"

#else

# define ALLOWED_TILES " 01NSEW"
# define WALKABLE_TILES "0NESW"

#endif

/*
** 函数名：validate_open_walls（static）
** 作用：检查一个“可走格”是否被墙正确包围，避免地图漏气（open wall）
** 参数：
**   game：提供 game->map 以及 map_h/map_w
**   i/j：当前格子的坐标（行 i，列 j）
** 主要逻辑（非常关键）：
**   1) 可走格不能出现在地图边界（第一行/最后一行/第一列/最后一列）
**   2) 上下左右任意方向如果是 ' '（空格），说明它挨着“地图外部/无效区域” => 漏气
** 调用者：
**   scan_map()：遇到 WALKABLE_TILES 就检查
*/
static void	validate_open_walls(t_game *game, int i, int j)
{
	/* 在边界上就一定漏气（因为边界外没有墙包） */
	if (i == 0 || j == 0 || i == game->map_h - 1 || j == game->map_w - 1)
		graceful_exit(game, 1, __func__, "Open wall found.");
	/* 上下是空格 => 漏气 */
	if (game->map[i - 1][j] == ' ' || game->map[i + 1][j] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
	/* 左右是空格 => 漏气 */
	if (game->map[i][j - 1] == ' ' || game->map[i][j + 1] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
}

/*
** 函数名：extract_player（static）
** 作用：遇到 N/E/S/W 出生点时：
**   1) 检查是否重复（只能有一个出生点）
**   2) 把玩家坐标与朝向写入 game->player
**   3) 把地图上的该字符改成 '0'（出生点属于可走地面）
** 参数：
**   game：写入 player 信息，并修改 map
**   i/j：出生点所在格子坐标
**   found：外面传进来的“是否已经找到过出生点”的标记
** 主要逻辑：
**   - x/y 用格子中心：j + 0.5 / i + 0.5
**   - angle 用 PI 的倍数表示四个方向
** 调用者：
**   scan_map()
*/
static void	extract_player(t_game *game, int i, int j, bool *found)
{
	if (*found)
		graceful_exit(game, 1, __func__, "Multiple start position.");
	*found = true;

	/* 玩家初始位置：格子中心点 */
	game->player.x = (float)j + 0.5f;
	game->player.y = (float)i + 0.5f;
	game->player.map_x = j;
	game->player.map_y = i;

	/* 根据字符决定朝向角度（单位是弧度，PI=3.1415...） */
	if (game->map[i][j] == 'E')
		game->player.angle = PI * 0.0f;
	if (game->map[i][j] == 'N')
		game->player.angle = PI * 0.5f;
	if (game->map[i][j] == 'W')
		game->player.angle = PI * 1.0f;
	if (game->map[i][j] == 'S')
		game->player.angle = PI * 1.5f;

	/* 出生点在地图里当作地面 */
	game->map[i][j] = '0';
}

/*
** 函数名：is_allowed_tile（static）
** 作用：判断字符 c 是否属于“允许出现的地图字符”
** 参数：
**   c：地图里的一个字符
** 返回：
**   true：允许
**   false：非法字符
** 调用者：
**   scan_map()
*/
static bool	is_allowed_tile(char c)
{
	return (ft_strchr(ALLOWED_TILES, c) != NULL);
}

/*
** 函数名：is_walkable_tile（static）
** 作用：判断字符 c 是否属于“可走格”
** 参数：
**   c：地图里的一个字符
** 返回：
**   true：可走（必须做 open wall 检查）
**   false：不可走（比如 '1' 墙 或 ' ' 外部）
** 调用者：
**   scan_map()
*/
static bool	is_walkable_tile(char c)
{
	return (ft_strchr(WALKABLE_TILES, c) != NULL);
}

/*
** 函数名：scan_map
** 作用：扫描整个 game->map，做三件大事：
**   1) 每个字符必须合法（ALLOWED_TILES）
**   2) 找到玩家出生点（N/E/S/W），且只能有一个
**   3) 所有可走格必须封闭（validate_open_walls）
** 参数：
**   game：提供 map / map_h/map_w，并写入 player 信息
** 主要逻辑：
**   双层循环遍历 map[i][j]：
**     - 先判非法字符
**     - 再遇到 NESW 提取玩家
**     - 再遇到可走格（含 0、出生点、bonus 的一些格子）检查是否漏气
**   最后如果没找到出生点 => 报错
** 调用者：
**   parse_map()
*/
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
		while (game->map[i][++j])
		{
			c = game->map[i][j];

			/* 1) 非法字符直接退出 */
			if (!is_allowed_tile(c))
				graceful_exit(game, 1, __func__, "Invalid map character.");

			/* 2) 出生点处理（并把该格改成 '0'） */
			if (ft_strchr("NESW", c))
				extract_player(game, i, j, &found);

			/* 3) 可走格必须被墙包围（不能挨着 ' ' 或边界） */
			if (is_walkable_tile(c))
				validate_open_walls(game, i, j);
		}
	}
	if (!found)
		graceful_exit(game, 1, __func__, "No start position found.");
}
