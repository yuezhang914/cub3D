/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/18 17:08:19 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/*
** 函数：normalize_angle
** 作用：把角度限制在 [-PI, PI]，避免 angle 无限变大/变小造成精度问题。
** 参数：
**   a：任意角度（弧度）
** 返回：
**   归一化后的角度（弧度）
** 调用：
**   on_mouse_move() 内部调用
*/
static float	normalize_angle(float a)
{
	while (a > PI)
		a -= 2.0f * PI;
	while (a < -PI)
		a += 2.0f * PI;
	return (a);
}

/*
** 函数：enable_mouse
** 作用：启用鼠标旋转：隐藏鼠标并把鼠标移动到窗口中心。
** 参数：
**   game：总结构体（需要 game->mlx / game->win）
** 调用：
**   setup_mlx() 最后调用一次（窗口创建好以后）
*/
void	enable_mouse(t_game *game)
{
	if (game == NULL || game->mlx == NULL || game->win == NULL)
		return ;
	if (game->player.mouse_enabled == 0)
		return ;
	mlx_mouse_hide(game->mlx, game->win);
	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
}

/*
** 函数：on_mouse_move
** 作用：鼠标移动事件回调：根据鼠标在 x 方向偏移，更新玩家视角 angle。
** 参数：
**   x, y：鼠标当前在窗口内的坐标（X11 MotionNotify）
**   game：总结构体（修改 game->player.angle）
** 返回：
**   0（mlx_hook 回调习惯返回 0）
** 调用：
**   setup_hooks() 里通过 mlx_hook(event=6, mask=1L<<6) 绑定
**
** 实现要点：
**   1) 用 dx = x - WIDTH/2 得到相对中心偏移
**   2) angle += dx * mouse_sens（只看水平移动）
**   3) 把鼠标强制拉回中心，保证不会跑出窗口
*/
int	on_mouse_move(int x, int y, t_game *game)
{
	int		dx;
	float	delta;

	(void)y;
	if (game == NULL)
		return (0);
	if (game->player.mouse_enabled == 0)
		return (0);
	if (x == WIDTH / 2)
		return (0);
	dx = x - (WIDTH / 2);
	delta = (float)dx * game->player.mouse_sens;
	game->player.angle = normalize_angle(game->player.angle + delta);
	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
