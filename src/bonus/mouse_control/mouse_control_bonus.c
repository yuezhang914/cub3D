/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 14:03:33 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：normalize_angle（static）
** 作用：把角度 a 规范到 [-PI, PI] 范围内
**
** 参数：
**   a：任意角度（弧度制）
**
** 返回：
**   规范后的角度，保证在 [-PI, PI]
**
** 主要逻辑（初中版理解）：
**   角度是“绕圈”的：
**   - 你转 360°（2PI）其实回到原方向
**   所以当 a 太大时，就减去 2PI；
**   当 a 太小时，就加上 2PI；
**   直到它落在 [-PI, PI]。
**
** 为什么要做这个？
**   如果不限制，angle 可能越来越大（比如几百万），
**   浮点数精度会变差，后面 cos/sin 可能不稳定。
**
** 调用者：
**   on_mouse_move() 更新 angle 后调用
*/
static float	normalize_angle(float a)
{
	while (a > PI)
		a -= 2.0f * PI;
	while (a < -PI)
		a += 2.0f * PI;
	return (a);
}

// /*
// ** 函数名：enable_mouse
// ** 作用：当你要启用鼠标控制时：
// **   1) 隐藏鼠标指针
// **   2) 把鼠标移动到窗口中心
// **
// ** 参数：
// **   game：需要 game->mlx / game->win 来调用 mlx 函数；
// **         并读取 game->player.mouse_enabled 判断是否启用
// **
// ** 主要逻辑：
// **   - 如果 game/mlx/win 为空：直接 return（避免崩溃）
// **   - 如果 mouse_enabled==0：说明不想启用鼠标，也 return
// **   - mlx_mouse_hide：隐藏鼠标（更像 FPS）
// **   - mlx_mouse_move：把鼠标强制放到屏幕中心
// **
// ** 调用者：
// **   一般在开始游戏/切换鼠标模式时调用（例如 setup 完或按键切换后）
// */
// void	enable_mouse(t_game *game)
// {
// 	if (game == NULL || game->mlx == NULL || game->win == NULL)
// 		return ;
// 	if (game->player.mouse_enabled == 0)
// 		return ;
// 	mlx_mouse_hide(game->mlx, game->win);
// 	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
// }

/*
** 函数名：on_mouse_move
** 作用：鼠标移动事件回调：根据鼠标左右移动来改变玩家视角 angle
**
** 参数：
**   x, y：鼠标在窗口内的坐标（像素）
**   game：游戏状态（读 mouse_enabled / mouse_sens，写 player.angle）
**
** 返回：
**   0：给 mlx 的回调返回值（表示事件处理完成）
**
** 主要逻辑（核心：只看水平偏移 dx）：
**   1) 忽略 y：因为本项目只做“左右转头”，不做“上下抬头低头”
**   2) 如果 game==NULL 或 mouse_enabled==0：不处理
**   3) 如果 x 正好是中心 WIDTH/2：说明没有水平偏移，不处理（避免死循环抖动）
**   4) dx = x - WIDTH/2
**   5) delta = dx * mouse_sens
**      - mouse_sens 是灵敏度，越大转得越快
**   6) angle = normalize_angle(angle + delta)
**   7) 把鼠标重新移回中心：
**      mlx_mouse_move(..., WIDTH/2, HEIGHT/2)
**      这样鼠标不会跑到屏幕边缘，玩家可以一直转
**
** 调用者：
**   setup_hooks() 中用 mlx_hook(event=6 MotionNotify) 绑定
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
	// mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);删掉鼠标回中 否则无法用鼠标关窗口
	return (0);
}
