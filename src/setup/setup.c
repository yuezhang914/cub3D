/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:05:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 19:05:37 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：init_game
** 作用：初始化 t_game 里的“默认值”，让后面 parse / mlx / 渲染能安全使用。
** 参数：
**   game：主结构体指针
** 用在哪：
**   main 一开始最先调用（在 parse 之前），把结构体清零并设置默认参数。
*/
void	init_game(t_game *game)
{
	float	fov_rad;

	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	/* 将 FOV（度）转弧度，再计算投影焦距（和 DDA 相机平面配套） */
	fov_rad = (float)(FOV * (PI / 180.0f));
	game->focal_length = (float)(2.0 * tan(fov_rad / 2.0f));
	game->pix_per_unit = (int)((float)MINI_HEIGHT / (DISTANCE_SEEN * 2.0f));
	/* 玩家默认移动参数（出生点由 parse_map 写入 x/y/angle） */
	game->player.move_speed = 0.06f;
	game->player.rotate_speed = 0.04f;
}

/*
** 函数：remember_image
** 作用：把一个 MLX image 指针登记到链表里，方便程序退出时统一销毁。
** 参数：
**   game：主结构体（里面有 img_head）
**   ptr ：mlx_xxx 返回的 image 指针
** 用在哪：
**   load_texture() 里，每加载一张 xpm 图，就把 img_ptr 记下来。
*/
void	remember_image(t_game *game, void *ptr)
{
	t_img	*new;

	new = track_malloc(game, sizeof(t_img));
	new->ptr = ptr;
	new->next = game->img_head;
	game->img_head = new;
}

/*
** 函数：setup_hooks
** 作用：给窗口绑定事件回调（键盘按下/松开、点击右上角关闭）。
** 参数：
**   game：主结构体（作为回调的上下文）
** 用在哪：
**   setup_mlx() 最后调用，保证窗口一出现就能响应按键和关闭事件。
*/
void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
}

/*
** 函数：setup_mlx
** 作用：初始化 MLX、创建窗口、创建主画布（img），拿到像素数组地址 data。
** 参数：
**   game：主结构体（写入 mlx/win/img/data 等字段）
** 用在哪：
**   parse 完成之后调用（因为要先知道贴图路径/颜色等），然后加载贴图并挂 hook。
*/
void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
		graceful_exit(game, 1, "setup_mlx", "Image creation failed");
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	// 加载纹理，内部也要有失败检查
	load_wall_textures(game);
	setup_hooks(game);
}
