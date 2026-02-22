/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 20:50:12 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:55:28 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS
/*
** 函数名：init_sprite_params（static，BONUS）
** 作用：初始化 sprite（物体/精灵）相关的默认参数，让 bonus 的物体渲染有合理默认值
** 参数：
**   game：写入 game->sprs（sprite 列表信息）和 game->config（每种 sprite 的配置）
** 主要逻辑：
**   1) sprs.num = 0：当前没有任何 sprite 实体
**   2) sprs.list = NULL：sprite 链表/数组还没分配
**   3) 把 config 数组清零（避免有垃圾值）
**   4) 给每种 sprite 设置默认缩放：h_div/v_div = 1
**   5) 给部分 sprite 设置默认上下偏移 v_move（显示位置更自然）
** 调用者：
**   init_game()（BONUS 版本）初始化时调用
*/
static void	init_sprite_params(t_game *game)
{
	int	i;

	game->sprs.num = 0;
	game->sprs.list = NULL;

	/* 清空每个 sprite 类型的配置结构体 */
	ft_bzero(game->config, sizeof(t_sprite_config) * SPR_COUNT);

	i = -1;
	while (++i < SPR_COUNT)
	{
		/* 默认不缩放：除数为 1 */
		game->config[i].h_div = 1.0f;
		game->config[i].v_div = 1.0f;

		/* 默认不上下移动 */
		game->config[i].v_move = 0.0f;
	}

	/* 给特定物体一个更好看的默认偏移（例：桶往下、火把往上） */
	game->config[SPR_BARREL].v_move = 200.0f;
	game->config[SPR_TORCH].v_move = -400.0f;
}
#endif

#ifdef BONUS
/*
** 函数名：init_game（BONUS）
** 作用：初始化整个 game 结构体的默认值（bonus 版本）
** 参数：
**   game：要初始化的 t_game 指针
** 主要逻辑：
**   1) ft_bzero：把整个结构体清零（避免野值导致随机崩溃）
**   2) 把 ceiling_color/floor_color 设为 -1：表示“还没从 cub 配置读到”
**   3) 设置玩家移动速度 move_speed 和旋转速度 rot_speed
**   4) bonus：默认开启鼠标控制 mouse_enabled，并设置灵敏度 mouse_sens
**   5) bonus：初始化 sprite 配置
**   6) pix_per_unit：地图单位到像素的比例（渲染时常用）
**   7) focal_length：由 FOV 计算的透视参数（tan 公式）
** 调用者：
**   main 的启动流程里最早调用（在 parse 之前也可以）
*/
void	init_game(t_game *game)
{
	/* 1) 清零，避免结构体里有随机垃圾值 */
	ft_bzero(game, sizeof(t_game));

	/* 2) -1 表示“还没有解析到颜色配置” */
	game->ceiling_color = -1;
	game->floor_color = -1;

	/* 3) 玩家运动默认值 */
	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
	game->player.angle = 0.0f;

	/* 4) BONUS：鼠标默认开启 */
	game->player.mouse_enabled = 1;
	game->player.mouse_sens = MOUSE_SENS;

	/* 5) BONUS：sprite 参数初始化 */
	init_sprite_params(game);

	/* 6) 渲染缩放：每 1 格地图单位对应多少像素（你项目里选 8） */
	game->pix_per_unit = 8.0f;

	/* 7) 透视参数：FOV（角度）转弧度后做 tan */
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
}
#else
/*
** 函数名：init_game（非 BONUS）
** 作用：初始化 game 默认值（基础版本）
** 参数：
**   game：要初始化的 t_game 指针
** 主要逻辑：
**   与 BONUS 相同的部分：
**   - 清零
**   - floor/ceiling = -1
**   - move_speed/rot_speed/angle
**   差别：
**   - mouse 默认关闭（mouse_enabled=0）
**   - 不初始化 sprite 参数（因为基础版没有 sprite）
** 调用者：
**   main 的启动流程
*/
void	init_game(t_game *game)
{
	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;

	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
	game->player.angle = 0.0f;

	/* 基础版不开鼠标 */
	game->player.mouse_enabled = 0;
	game->player.mouse_sens = 0.0f;

	game->pix_per_unit = 8.0f;
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
}
#endif

/*
** 函数名：remember_image
** 作用：记录一个 MLX image 指针到链表中，方便程序退出时统一释放，避免内存泄漏
** 参数：
**   game：里面有链表头 game->img_head
**   ptr：要记录的 mlx image 指针（例如贴图、离屏画布）
** 主要逻辑：
**   1) ptr 为 NULL 就不处理（加载失败时避免崩溃）
**   2) 分配一个链表节点，保存 ptr
**   3) 头插法插入到 game->img_head
** 调用者：
**   texture.c 的 load_texture() 加载贴图后调用
**   以及任何创建 mlx image 的地方都可以调用
*/
void	remember_image(t_game *game, void *ptr)
{
	t_img	*new_node;

	if (ptr == NULL)
		return ;

	new_node = (t_img *)track_malloc(game, sizeof(t_img));
	new_node->ptr = ptr;

	/* 头插：新节点指向旧头，再把头更新为新节点 */
	new_node->next = game->img_head;
	game->img_head = new_node;
}
