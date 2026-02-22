/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_view_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 23:10:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 12:26:05 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "cub3d.h"

/*
** 函数：load_door_texture
** 作用：加载门贴图（独立于 NO/SO/WE/EA）。
** 参数：
**   game：总结构体（写入 game->door）
** 调用：
**   handle_bonus_setup()
** 说明：
**   你可以把路径改成你自己的门贴图 xpm。
*/
void	load_door_texture(t_game *game)
{
	if (!game)
		return ;
	game->door.path = "assets/door/door_close.xpm";
	load_texture(game, &game->door);

	game->door_open.path = "assets/door/door_open.xpm";
	load_texture(game, &game->door_open);
}

/*
** 函数：door_ray_blocks
** 作用：
**   DDA 进入某个 'D' 门格子后，判断当前射线打到的是“门洞”还是“门板”
**   - door_state=0（关）：总是门板 -> 挡
**   - door_state=1（开）：总是门洞 -> 不挡
** 参数：
**   game：总结构体
**   d：DDA 状态（需用到 side 与 perp_dist）
**   rdx,rdy：射线方向
** 返回：
**   1=挡（命中门板），0=不挡（穿门洞）
** 调用：
**   perform_dda()（BONUS 分支）
*/

int	door_ray_blocks(t_game *game, t_dda *d, float rdx, float rdy)
{
	float	open;
	float	wall_x;

	if (!game || !game->door_state)
		return (1);

	open = (float)game->door_state[d->map_y][d->map_x]; /* 0 或 1 */

if (open <= 0.0f)   // 关门
    return (1);
if (open >= 1.0f)   // 开门
    return (0);


	/* 如果你未来做 0~1 动画，这段才会用到 */
	if (d->side == 0)
		wall_x = game->player.y + d->perp_dist * rdy;
	else
		wall_x = game->player.x + d->perp_dist * rdx;
	wall_x = wall_x - floorf(wall_x);
	if (wall_x < open)
		return (0);
	return (1);
}

