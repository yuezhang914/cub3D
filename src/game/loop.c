/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:42:58 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 21:04:50 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：game_step
** 作用：每一帧调用一次，更新玩家 → 处理复活 → 清屏 → 画3D墙 → 画小地图 → 显示到窗口。
** 参数：
**   game：总结构体（包含地图、玩家、mlx、img 等）
** 返回：
**   0（mlx_loop_hook 需要 int 返回值）
** 用在哪：
**   mlx_loop_hook(game->mlx, game_step, game) 绑定主循环。
*/
int	game_step(t_game *game)
{
	update_player(game);
	// respawn_update(game);
	clear_image(game);
	/* 3D 场景：你自己的墙渲染（保持不变） */
	draw_walls(game);
	/* 2D 叠加：队友的小地图（合并进你的主循环） */
	// render_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	// draw_win_logo_if_finished(game);
	return (0);
}

/*
** 函数：on_window_close
** 作用：点窗口右上角关闭按钮时退出程序。
** 参数：
**   param：mlx_hook 传进来的指针，这里强转成 t_game*
** 返回：
**   1（随便返回一个非 0 也行）
** 用在哪：
**   mlx_hook(win, 17, 0, on_window_close, game)
*/
int	on_window_close(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	graceful_exit(game, 0, NULL, NULL);
	return (1);
}
