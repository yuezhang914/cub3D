/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:46:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:42:53 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：check_map_is_last
** 作用：保证地图是文件最后一段：map 结束后只能有空行，不能再出现别的内容
** 参数：
**   game：用于 set_input_line_type / graceful_exit
**   lines：整个 .cub 的行数组
**   start：map 开始的行号（index）
** 主要逻辑：
**   1) 从 start 开始，连续跳过所有 MAP 行
**   2) map 之后允许有 EMPTY 行（空行），继续跳过
**   3) 如果后面还剩下非空内容 => 报错
** 调用者：
**   parse_map()
*/
void	check_map_is_last(t_game *game, char **lines, int start)
{
	int	i;

	i = start;
	while (lines[i] && set_input_line_type(game, lines[i]) == MAP)
		i++;
	while (lines[i] && set_input_line_type(game, lines[i]) == EMPTY)
		i++;
	if (lines[i] != NULL)
		graceful_exit(game, 1, __func__, "Non-empty line after map.");
}

/*
** 函数名：set_map_dimensions
** 作用：计算 map 的高度 map_h 和最大宽度 map_w
** 参数：
**   game：把结果写入 game->map_h / game->map_w
**   lines：整个 .cub 的行数组
**   start：map 开始的行号
** 主要逻辑：
**   1) 从 start 开始，遇到 MAP 行就算进高度
**   2) 每一行用 ft_strlen 算长度，更新最大宽度 map_w
**   3) 遇到非 MAP 行就停止（map 结束）
** 调用者：
**   parse_map()
*/
void	set_map_dimensions(t_game *game, char **lines, int start)
{
	int	i;
	int	w;

	i = start;
	while (lines[i] && set_input_line_type(game, lines[i]) == MAP)
	{
		w = (int)ft_strlen(lines[i]);
		if (w > game->map_w)
			game->map_w = w;
		game->map_h++;
		i++;
	}
}

/*
** 函数名：find_map_start
** 作用：在整个文件里找到第一行 MAP 的位置（index）
** 参数：
**   game：用于 set_input_line_type / graceful_exit
**   lines：整个 .cub 的行数组
** 返回：
**   start：map 的第一行 index
** 主要逻辑：
**   从 0 往后找，直到某行类型是 MAP
**   如果找不到 MAP 行 => 报错
** 调用者：
**   parse_map()
*/
int	find_map_start(t_game *game, char **lines)
{
	int	start;

	start = 0;
	while (lines[start] && set_input_line_type(game, lines[start]) != MAP)
		start++;
	if (lines[start] == NULL)
		graceful_exit(game, 1, __func__, "No map line found.");
	return (start);
}
