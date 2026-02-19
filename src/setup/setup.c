/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:05:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 10:38:04 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：init_game
** 作用：把 t_game 初始化到“安全默认状态”，保证：
**   1) 指针默认为 NULL（方便 graceful_exit 判断是否需要 destroy）
**   2) 必做所需的玩家速度、颜色默认值设置好
** 参数：
**   game：总结构体指针
*/
void init_game(t_game *game)
{
	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	/* 玩家移动参数：必做需要 */
	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
#ifdef BONUS
	game->player.mouse_enabled = 1;
	game->player.mouse_sens = MOUSE_SENS;
#else
	game->player.mouse_enabled = 0;
	game->player.mouse_sens = 0.0f;
#endif
	/*
	** 小地图相关（必做不一定用到）
	** 为了不依赖 DISTANCE_SEEN，这里给一个保底值：
	** pix_per_unit 越大，小地图越“放大”
	*/
	game->pix_per_unit = 8.0f;
	/* 如果你 DDA 相机模型用 focal_length，可保留默认值 */
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
#ifdef BONUS
	game->sprs.num = 0;
	game->sprs.list = NULL;
	// 假设你的结构体里有 t_sprite_manager sprs;
	ft_bzero(game->sprs.config, sizeof(t_sprite_config) * SPR_COUNT);
#endif
}

/*
** 函数：remember_image
** 作用：把一个 mlx image 指针记录进链表，方便退出时统一 mlx_destroy_image。
** 参数：
**   game：总结构体（保存链表头 img_head）
**   ptr ：要记录的 mlx image 指针
*/
void remember_image(t_game *game, void *ptr)
{
	t_img *new;

	if (ptr == NULL)
		return;
	new = (t_img *)track_malloc(game, sizeof(t_img));
	new->ptr = ptr;
	new->next = game->img_head;
	game->img_head = new;
}

/*
** 函数：setup_hooks
** 作用：绑定键盘按下/松开事件与窗口关闭事件
** 参数：
**   game：总结构体
*/
void setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
#ifdef BONUS
	mlx_hook(game->win, 6, 1L << 6, on_mouse_move, game);
#endif
}

/*
** 函数：setup_mlx
** 作用：初始化 mlx、创建窗口与主画布，加载贴图并挂 hooks
** 参数：
**   game：总结构体
*/
void setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
								   &game->endian);
	if (game->data == NULL)
		graceful_exit(game, 1, __func__, "mlx_get_data_addr failed.");
	load_wall_textures(game);
	load_wall_textures(game);
#ifdef BONUS
	load_door_texture(game);
	/* ✅ 如果地图里有精灵，再加载精灵贴图（现在 mlx 已经 ready） */
	if (game->sprs.num > 0)
		init_sprite_texture(game);
#endif
	setup_hooks(game);
}
