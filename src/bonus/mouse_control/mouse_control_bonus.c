/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 23:41:30 by yzhang2          ###   ########.fr       */
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
** 函数名：on_mouse_move
** 作用：
**   鼠标移动事件回调：用“本次鼠标 x 与上次 x 的差值 dx”来控制玩家左右转身（改变 angle）。
**   目标是：
**     - 不强制把鼠标回中（鼠标可离开窗口去点退出按钮）
**     - 通过 EDGE 处理“贴边后 x 不变导致无法继续转身”的问题
**     - 通过 MAX_DX 限幅处理“鼠标抖一下转太大”的问题
**
** 参数：
**   x, y：
**     鼠标在窗口内的坐标（像素）。这里只用 x（水平移动），y 不参与计算（忽略上下抬头/低头）。
**   game：
**     游戏总状态指针。读取：
**       - game->player.mouse_enabled：是否启用鼠标控制
**       - game->player.mouse_sens：鼠标灵敏度（每像素移动对应旋转多少弧度）
**     写入：
**       - game->player.angle：玩家朝向角（弧度）
**
** 常量（本函数内部可调）：
**   EDGE：
**     边缘阈值（像素）。当鼠标 x <= EDGE 或 x >= WIDTH-1-EDGE 时认为“贴边”。
**     贴边时把 last_x 推回内部一点，保证后续还能产生 dx。
**   MAX_DX：
**     dx 的最大绝对值（像素）。限制单次事件的最大转身速度，避免偶发大跳变。
**
** 主逻辑：
**   1) 用 static last_x 记住上一次回调的 x（第一次进入只记录，不旋转）
**   2) dx = x - last_x
**   3) clamp(dx)：把 dx 限制在 [-MAX_DX, +MAX_DX]
**   4) angle += dx * mouse_sens，并用 normalize_angle() 把角度收敛到合理范围
**   5) 更新 last_x：
**        - 若贴边：last_x 推回内部一点（“软修复”，不移动真实鼠标指针）
**        - 否则：last_x = x
**
** 调用：
**   通过 mlx_hook 绑定鼠标移动事件（X11 MotionNotify）：
**     mlx_hook(game->win, 6, 1L << 6, on_mouse_move, game);
*/

int on_mouse_move(int x, int y, t_game *game)
{
    static int last_x = -1;          // 记录上一次鼠标事件的 x；static 表示跨回调保存
    const int EDGE = 2;              // “贴边阈值”：离左右边缘 <=2 像素就认为贴边
    const int MAX_DX = 40;           // dx 限幅：单次事件最多按 40 像素来计算旋转
    int dx;                          // 本次相对移动量：x - last_x

    (void)y;                         // 只做水平转身，不用 y；避免 unused warning
    if (!game || game->player.mouse_enabled == 0)  // game 为空或未启用鼠标控制
        return 0;                    // 不做任何处理

    // 第一次进入回调：last_x 还没初始化
    // 只记录，不旋转，避免第一次 dx 特别大（比如从窗口外进来）
    if (last_x < 0)
    {
        last_x = x;                  // 记住当前 x 作为“起点”
        return 0;                    // 这一帧不旋转
    }

    dx = x - last_x;                 // 计算本次鼠标水平移动量（相对上一次）

    // clamp：限制一次事件的最大旋转量，避免鼠标抖一下转太大
    if (dx > MAX_DX) dx = MAX_DX;    // dx 太大 → 截断到 MAX_DX
    if (dx < -MAX_DX) dx = -MAX_DX;  // dx 太小 → 截断到 -MAX_DX

    // 用 dx * 灵敏度 来改变角度（弧度）；再 normalize 防止角度无限增大
    game->player.angle = normalize_angle(
        game->player.angle + (float)dx * game->player.mouse_sens
    );

    // 关键：贴边修复
    // 鼠标到窗口边缘后，系统会把 x 卡住（不再变），导致 dx=0 无法继续转身
    // 解决：不改变真实鼠标指针位置，只把 last_x 人为推回内部一点
    if (x <= EDGE)                               // 贴左边缘
        last_x = EDGE + 1;                       // 把 last_x 推到边缘内侧
    else if (x >= WIDTH - 1 - EDGE)              // 贴右边缘
        last_x = WIDTH - 2 - EDGE;               // 同理推回内侧
    else                                         // 没贴边：正常更新
        last_x = x;                              // last_x 直接等于当前 x

    return 0;                                    // 告诉 mlx：事件已处理
}
