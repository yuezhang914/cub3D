/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 14:24:03 by yzhang2          ###   ########.fr       */
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
/*
** 函数：on_mouse_move
** 作用：
**   BONUS：根据鼠标左右移动来旋转玩家视角（第一人称“转头”）。
**   这版实现“不把鼠标强制拉回窗口中心”，所以鼠标可以移到窗口外点击红叉关闭。
**
** 参数：
**   x, y  ：当前鼠标在窗口内的坐标（像素）。
**           本函数只使用 x（左右方向），y 不参与视角旋转。
**   game  ：全局游戏结构体，读取 player.mouse_enabled / player.mouse_sens，
**           并更新 player.angle。
**
** 常量/关键字段：
**   last_x（static）：
**     - 保存“上一次回调时的鼠标 x 坐标”，用于计算增量 dx。
**     - 这样旋转速度只由“鼠标移动了多少”决定，而不是“离中心多远”。
**   dx dead-zone（微小移动死区）：
**     - 如果 dx 很小（例如 -1~1），直接忽略，减少抖动与噪声。
**   dx clamp（最大旋转上限）：
**     - 限制单次 dx 的最大值（例如 ±60），防止窗口外/系统跳变导致瞬间巨转。
**   game->player.mouse_sens：
**     - 灵敏度系数：delta = dx * mouse_sens。
**     - 建议范围：0.002f ~ 0.006f（按手感微调）。
**   normalize_angle：
**     - 把角度规范到 [-PI, PI] 范围，避免数值无限变大。
**
** 主要逻辑：
**   1) 若 game 为空或 mouse 未开启，直接返回 0。
**   2) 第一次收到鼠标事件时，只初始化 last_x 并返回（避免第一次 dx 乱跳）。
**   3) dx = x - last_x；更新 last_x。
**   4) 若 dx 落在死区范围内，忽略。
**   5) 将 dx 限幅到 [-MAX_DX, MAX_DX]（例如 ±60）。
**   6) delta = dx * mouse_sens；更新 player.angle，并 normalize。
**
** 返回值：
**   0（按 MLX hook 约定返回 0 即可）
**
** 在哪调用：
**   通过 mlx_hook 绑定到鼠标移动事件（MotionNotify）：
**     mlx_hook(game->win, 6, 1L << 6, on_mouse_move, game);
*/
int	on_mouse_move(int x, int y, t_game *game)
{
	static int	last_x = -1;
	int			dx;
	float		delta;

	(void)y;
	if (game == NULL)
		return (0);
	if (game->player.mouse_enabled == 0)
		return (0);
	if (last_x == -1)
	{
		last_x = x;
		return (0);
	}
	dx = x - last_x;
	last_x = x;
	if (dx > -1 && dx < 1)
		return (0);
	if (dx > 60)
		dx = 60;
	if (dx < -60)
		dx = -60;
	delta = (float)dx * game->player.mouse_sens;
	game->player.angle = normalize_angle(game->player.angle + delta);
	return (0);
}
