/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:03:31 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/21 00:03:48 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：init_sprite_params (static)
** 作用：为 Bonus 模式下的所有精灵设置安全的物理默认值。
** 参数：game：总结构体指针。
** 调用：被 init_game 调用。
** 主要逻辑：
** 1. 遍历所有精灵配置，初始化缩放比例和垂直位移。
** 2. 针对特定物体（如桶、火炬）进行视觉高度修正。
*/
#ifdef BONUS

static void	init_sprite_params(t_game *game)
{
	int	i;

	game->sprs.num = 0;
	game->sprs.list = NULL;
	ft_bzero(game->config, sizeof(t_sprite_config) * SPR_COUNT);
	i = -1;
	while (++i < SPR_COUNT)
	{
		game->config[i].h_div = 1.0f;
		game->config[i].v_div = 1.0f;
		game->config[i].v_move = 0.0f;
	}
	game->config[SPR_BARREL].v_move = 200.0f;
	game->config[SPR_TORCH].v_move = -400.0f;
}
#endif

/*
** 函数：init_game
** 作用：将游戏状态初始化为安全的默认值，防止空指针崩溃。
** 参数：game：总结构体指针。
** 调用：在程序启动初期调用。
** 主要逻辑：
** 1. 清空结构体并设置颜色、速度等核心参数。
** 2. 计算焦距（Focal Length）以确定视野范围。
** 3. 若开启 Bonus，则进一步初始化精灵配置。
*/
void	init_game(t_game *game)
{
	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
	game->player.angle = 0.0f;
	game->player.mouse_enabled = 0;
	game->player.mouse_sens = 0.0f;
#ifdef BONUS
	game->player.mouse_enabled = 1;
	game->player.mouse_sens = MOUSE_SENS;
	init_sprite_params(game);
#endif
	game->pix_per_unit = 8.0f;
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
}

/*
** 函数：remember_image
** 作用：利用链表追踪所有创建的 MLX 图像指针。
** 参数：game：总结构体；ptr：MLX 图像指针。
** 调用：在每次 mlx_new_image 后调用。
** 主要逻辑：将指针存入新节点，并将其插入追踪链表的头部。
*/
void	remember_image(t_game *game, void *ptr)
{
	t_img	*new_node;

	if (ptr == NULL)
		return ;
	new_node = (t_img *)track_malloc(game, sizeof(t_img));
	new_node->ptr = ptr;
	new_node->next = game->img_head;
	game->img_head = new_node;
}

/*
** 函数：setup_hooks
** 作用：绑定所有的 MLX 事件回调函数（键盘、鼠标、关闭窗口）。
** 参数：game：总结构体指针。
** 调用：在 setup_mlx 结尾调用。
** 主要逻辑：绑定键盘按下/释放和窗口交叉按钮事件。
*/
void	setup_hooks(t_game *game)
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
** 作用：初始化图形库环境，创建窗口并加载资源。
** 参数：game：总结构体指针。
** 调用：在解析完地图文件后调用。
** 主要逻辑：
** 1. 初始化 MLX 库并加载贴图资源。
** 2. 创建窗口及主渲染画布。
** 3. 获取画布数据地址并挂载钩子。
*/
void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");
	load_wall_textures(game);
#ifdef BONUS
	load_door_texture(game);
	init_sprite_texture(game);
#endif
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	setup_hooks(game);
}
