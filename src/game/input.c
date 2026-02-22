/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:12:30 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

/*
** 函数名：handle_interact_key（static，BONUS）
** 作用：处理交互键（这里是按 E）：尝试开/关门
**
** 参数：
**   game：游戏总状态（try_toggle_door 需要玩家位置、地图、门状态等）
**
** 主要逻辑：
**   直接调用 try_toggle_door(game)：
**   - 如果玩家面前/附近有门，就切换门的开关状态
**
** 调用者：
**   on_key_down()：当按下 'e' 时调用
*/
static void	handle_interact_key(t_game *game)
{
	try_toggle_door(game);
}

#else

/*
** 函数名：handle_interact_key（static，非 BONUS）
** 作用：基础版没有门系统，所以按 E 不做任何事
**
** 参数：
**   game：不用，为了避免编译警告，用 (void)game
**
** 调用者：
**   on_key_down()
*/
static void	handle_interact_key(t_game *game)
{
	(void)game;
}

#endif

/*
** 函数名：on_key_down
** 作用：键盘“按下”事件回调：把按键状态写到 game->player 的 key_xxx 标记里
**
** 参数：
**   keycode：按键码（X11：XK_w, XK_Left 等）
**   game：游戏总状态（要修改 game->player.key_xxx）
**
** 主要逻辑（按键→状态）：
**   - ESC：立即退出（graceful_exit）
**   - w/s/a/d：设置移动方向标记（按住就一直为 1）
**   - Left/Right：设置旋转标记
**   - e：调用 handle_interact_key（BONUS：门交互）
**
** 调用者：
**   setup_hooks() 里用 mlx_hook 绑定（你项目其他文件里）
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
	else if (keycode == XK_e)
		handle_interact_key(game);
	return (0);
}

/*
** 函数名：on_key_up
** 作用：键盘“松开”事件回调：把对应按键状态清零
**
** 参数：
**   keycode：按键码
**   game：游戏总状态（要修改 key_ 标记）
**
** 主要逻辑：
**   - 松开 w/s/a/d 就把对应 key_xxx 设为 0
**   - 松开左右方向键就把旋转标记设为 0
**
** 抽象概念解释（为什么这样写）：
**   游戏不是“按一下移动一次”，而是：
**   - 按下：状态=1
**   - 每帧根据状态移动一点
**   - 松开：状态=0
**   这样按住键才能连续走
**
** 调用者：
**   setup_hooks() 里用 mlx_hook 绑定
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
