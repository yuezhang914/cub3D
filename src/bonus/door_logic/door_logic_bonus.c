/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_logic_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:28:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/18 23:11:04 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/*
** 函数：is_door_closed
** 作用：判断地图格 (x, y) 是否是一扇“关闭的门”。
** 参数：
**   game：总结构体（读取 map 与 door_state）
**   x, y：地图格坐标
** 返回：
**   1=关闭门，0=不是门/门已打开/越界
** 调用：
**   movement.c 的碰撞检测（BONUS）
**   raycasting_cal.c 的 DDA 命中判断（BONUS）
*/
int door_is_closed(t_game *game, int x, int y)
{
    if (!game || !game->door_state || !game->map)
        return (0);
    if (x < 0 || y < 0 || x >= game->map_w || y >= game->map_h)
        return (0);
    if (game->map[y][x] != 'D')
        return (0);
    return (game->door_state[y][x] <= 0.0f);
}


/*
** 函数：init_doors
** 作用：为所有门 'D' 建立 door_state 数组，并把初始状态设置为“关闭(0)”。
** 参数：
**   game：总结构体（写入 game->door_state）
** 调用：
**   handle_bonus_setup()（bonus 初始化入口）
*/
void	init_doors(t_game *game)
{
	int	y;

	if (!game || !game->map)
		return ;
	game->door_state = (unsigned char **)track_malloc(game,
								sizeof(unsigned char *) * game->map_h);
	y = 0;
	while (y < game->map_h)
	{
		game->door_state[y] = (unsigned char *)track_malloc(game,
									sizeof(unsigned char) * game->map_w);
		ft_bzero(game->door_state[y], (size_t)game->map_w);
		y++;
	}
}



static t_int_xy	door_target_tile(t_game *game)
{
	t_int_xy	pos;
	float		rx;
	float		ry;

	rx = game->player.x + cosf(game->player.angle) * 0.75f;
	ry = game->player.y + sinf(game->player.angle) * 0.75f;
	pos.x = (int)rx;
	pos.y = (int)ry;
	return (pos);
}

/*
** 函数：try_toggle_door
** 作用：尝试打开/关闭“玩家正前方一格”的门。
** 参数：
**   game：总结构体
** 调用：
**   on_key_down() 内：按下 E 时调用（BONUS）
** 主逻辑：
**   1) 取玩家前方 0.75 格的位置 -> 取整得到目标格
**   2) 如果那格是门 'D'，door_state 在 0/1 之间切换
*/
void	try_toggle_door(t_game *game)
{
	t_int_xy	p;

	if (!game || !game->door_state)
		return ;
	p = door_target_tile(game);
	if (p.x < 0 || p.y < 0 || p.x >= game->map_w || p.y >= game->map_h)
		return ;
	if (game->map[p.y][p.x] != 'D')
		return ;
	game->door_state[p.y][p.x] = (unsigned char)(!game->door_state[p.y][p.x]);
}
