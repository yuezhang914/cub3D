/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:42:58 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/18 01:48:07 by yzhang2          ###   ########.fr       */
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
int game_step(t_game *game)
{
	update_player(game);
	clear_image(game);

	/* 1. 渲染 3D 墙体（这里面会填充你的 z_buffer） */
	draw_loop(game);

	/* 2. 渲染 3D 精灵 (Bonus 逻辑) */
	#ifdef BONUS
	if (game->sprs.num > 0)
	{
		// A. 计算所有精灵到玩家的当前距离
		calculate_sprite_distance(game);
		// B. 按距离从远到近排序（防止透明度错位）
		sort_sprites(game);
		// C. 投影并绘制到画布上
		render_sprites(game);
	}
	/* 3. 渲染 2D 叠加层（小地图永远在最上方） */
	render_minimap(game);
	#endif

	

	/* 4. 将最终画布推送到窗口 */
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);

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
int on_window_close(void *param)
{
	t_game *game;

	game = (t_game *)param;
	graceful_exit(game, 0, NULL, NULL);
	return (1);
}
