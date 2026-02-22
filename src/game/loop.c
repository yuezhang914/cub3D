/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:42:58 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/20 19:05:45 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

static void handle_bonus_rendering(t_game *game)
{
	if (game->sprs.num > 0)
	{
		sort_sprites(game);
		render_sprites(game);
	}
	render_minimap(game);
}

#else

static void handle_bonus_rendering(t_game *game)
{
	(void)game;
}
#endif

int game_step(t_game *game)
{
	game->time += 0.016f;
	update_player(game);
	clear_image(game);
	draw_loop(game);
	/* 这里的调用在不同模式下会有不同的行为 */
	handle_bonus_rendering(game);
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
