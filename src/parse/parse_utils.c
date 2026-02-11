/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:46:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 11:46:48 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"
#include "func.h"
/*
** 作用：地图段后面不能出现“非空行”
** 逻辑：找到地图开始 -> 走过地图 -> 走过空行 -> 如果还有东西就是错
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
** 作用：统计地图高度(map_h)和最大宽度(map_w)
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
** 作用：在 lines（整个 .cub 的按行数组）里找到“地图段”的第一行位置。
**      地图段的判定依据是：set_input_line_type(...) == MAP。
**      如果一直找不到 MAP，说明 .cub 没有地图，直接报错退出。
**
** 参数：
**   game  ：全局上下文（用于 set_input_line_type 和 graceful_exit）
**   lines ：.cub 文件按行切分得到的 char**（以 NULL 结尾）
**
** 返回：
**   返回地图开始行的下标 start（int）
**
** 用在哪里：
**   parse_map() 的开头调用，用它确定地图从哪一行开始。
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

