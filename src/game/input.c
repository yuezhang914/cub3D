/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/18 22:07:18 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：on_key_down
** 作用：按键按下事件：只记录“按键状态”，不要在这里直接移动玩家。
** 参数：
**   keycode：按键码（X11）
**   game：总结构体（修改 game->player.key_*）
** 返回：
**   0（mlx_hook 回调一般返回 0）
*/
int	on_key_down(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		graceful_exit(game, 0, NULL, NULL);
	else if (keycode == XK_w)
		game->player.key_up = 1;
	else if (keycode == XK_s)
		game->player.key_down = 1;
	else if (keycode == XK_a)
		game->player.key_left = 1;
	else if (keycode == XK_d)
		game->player.key_right = 1;
	else if (keycode == XK_Left)
		game->player.key_rot_l = 1;
	else if (keycode == XK_Right)
		game->player.key_rot_r = 1;
#ifdef BONUS
	else if (keycode == XK_e)
		door_try_toggle(game);
#endif
	return (0);
}

/*
** 函数：on_key_up
** 作用：按键松开事件：把对应按键状态清零。
** 参数：
**   keycode：按键码（X11）
**   game：总结构体（修改 game->player.key_*）
** 返回：
**   0
*/
int	on_key_up(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->player.key_up = 0;
	else if (keycode == XK_s)
		game->player.key_down = 0;
	else if (keycode == XK_a)
		game->player.key_left = 0;
	else if (keycode == XK_d)
		game->player.key_right = 0;
	else if (keycode == XK_Left)
		game->player.key_rot_l = 0;
	else if (keycode == XK_Right)
		game->player.key_rot_r = 0;
	return (0);
}
