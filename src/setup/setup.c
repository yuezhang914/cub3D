/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:03:31 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:56:08 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS
/*
** 函数名：setup_hooks（BONUS）
** 作用：给窗口绑定事件回调（键盘/鼠标/关闭窗口）
** 参数：
**   game：作为回调函数的上下文参数传给回调
** 主要逻辑：
**   mlx_hook(window, event, mask, handler, param)
**   - event=2：按键按下 KeyPress → on_key_down
**   - event=3：按键松开 KeyRelease → on_key_up
**   - event=17：窗口关闭 DestroyNotify → on_window_close
**   - event=6：鼠标移动 MotionNotify → on_mouse_move（BONUS）
** 调用者：
**   setup_mlx()
*/
void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
	mlx_hook(game->win, 6, 1L << 6, on_mouse_move, game);
}
#else
/*
** 函数名：setup_hooks（非 BONUS）
** 作用：只绑定键盘与关闭窗口事件（无鼠标移动）
** 参数：
**   game：回调上下文
** 调用者：
**   setup_mlx()
*/
void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
}
#endif

#ifdef BONUS
/*
** 函数名：setup_mlx（BONUS）
** 作用：把 MLX 渲染环境搭建完整：mlx → 贴图 → 窗口 → 离屏画布 → 像素指针 → hooks
** 参数：
**   game：写入字段：
**     - mlx：mlx 环境指针
**     - win：窗口指针
**     - img：离屏画布 image
**     - data：画布像素数组指针
**     - bpp/size_line/endian：像素格式信息
** 主要逻辑（顺序不能乱）：
**   1) mlx_init：先获得 mlx
**   2) load_wall_textures/load_door_texture/init_sprite_texture：
**      因为加载 xpm 需要 mlx 指针，所以必须在 mlx_init 之后
**   3) mlx_new_window：创建窗口
**   4) mlx_new_image：创建离屏画布
**   5) mlx_get_data_addr：拿到离屏画布像素数组 data（每帧写这个数组）
**   6) setup_hooks：绑定键鼠事件
** 调用者：
**   启动流程（main 里 parse 完后通常会调用它）
*/
void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");

	/* BONUS：墙/门/sprite 都要加载 */
	load_wall_textures(game);
	load_door_texture(game);
	init_sprite_texture(game);

	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");

	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");

	/* data 是画布的像素数组指针，渲染时会写它 */
	game->data = mlx_get_data_addr(game->img,
			&game->bpp, &game->size_line, &game->endian);

	setup_hooks(game);
}
#else
/*
** 函数名：setup_mlx（非 BONUS）
** 作用：基础版 MLX 初始化（只需要墙贴图 + 窗口 + 画布 + hooks）
** 参数：
**   game：写入 mlx/win/img/data 等字段
** 主要逻辑：
**   1) mlx_init
**   2) load_wall_textures（只有墙）
**   3) mlx_new_window
**   4) mlx_new_image
**   5) mlx_get_data_addr
**   6) setup_hooks（无鼠标移动）
** 调用者：
**   启动流程
*/
void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");

	load_wall_textures(game);

	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");

	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");

	game->data = mlx_get_data_addr(game->img,
			&game->bpp, &game->size_line, &game->endian);

	setup_hooks(game);
}
#endif
