/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：on_key_down
** 作用：按键按下时，把状态写进 game->move（只记状态，不在这里移动玩家）。
** 参数：
**   keycode：X11 键码
**   game：总结构体
** 返回：
**   0
** 用在哪：
**   mlx_hook(win, 2, 1L<<0, on_key_down, game)
*/
int	on_key_down(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		graceful_exit(game, 0, NULL, NULL);
	if (keycode == XK_w)
		game->player.key_up = true;
	if (keycode == XK_s)
		game->player.key_down = true;
	if (keycode == XK_a)
		game->player.key_left = true;
	if (keycode == XK_d)
		game->player.key_right = true;
	if (keycode == XK_Left)
		game->player.left_rotate = true;
	if (keycode == XK_Right)
		game->player.right_rotate = true;
	if (keycode == XK_Return)
		debug_info(game);
	return (0);
}

/*
** 函数：on_key_up
** 作用：按键松开时，把状态改回 false。
** 参数：
**   keycode：X11 键码
**   game：总结构体
** 返回：
**   0
** 用在哪：
**   mlx_hook(win, 3, 1L<<1, on_key_up, game)
*/
int	on_key_up(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->player.key_up = false;
	if (keycode == XK_s)
		game->player.key_down = false;
	if (keycode == XK_a)
		game->player.key_left = false;
	if (keycode == XK_d)
		game->player.key_right = false;
	if (keycode == XK_Left)
		game->player.left_rotate = false;
	if (keycode == XK_Right)
		game->player.right_rotate = false;
	return (0);
}

/*
** 函数：on_mouse_move（BONUS）
** 作用：鼠标左右移动控制视角旋转，然后把鼠标强制拉回屏幕中心。
** 参数：
**   x, y：鼠标坐标（这里只用 x）
**   param：强转成 t_game*
** 返回：
**   0
** 用在哪：
**   mlx_hook(win, 6, 1L<<6, on_mouse_move, game)（如果你启用鼠标控制）
*/
int	on_mouse_move(int x, int y, void *param)
{
	t_game	*game;
	float	dx;
	float	angle;

	game = (t_game *)param;
	(void)y;
	dx = (x - WIDTH / 2);
	angle = (dx / WIDTH) * FOV;
	game->player.angle -= angle;
	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
