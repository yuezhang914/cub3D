/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:12 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:35:14 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_static_sprites(t_game *game)
{
	/* --- 树木 (TREE) 配置 --- */
	/* 只有一帧，不需要动画循环 */
	game->config[SPR_TREE].frame_count = 1;
	game->config[SPR_TREE].is_animated = false;
	/* 无论从哪个角度看，树的外观都一样（非 8 方向素材） */
	game->config[SPR_TREE].is_directional = false;
	/* 缩放比例为 1.0，意味着树将撑满整个墙高（通常用于大型背景物） */
	game->config[SPR_TREE].v_div = 1.0f;
	game->config[SPR_TREE].h_div = 1.0f;
	/* 垂直偏移为 0，底部与地面平齐 */
	game->config[SPR_TREE].v_move = 0.0f;

	/* --- 木桶 (BARREL) 配置 --- */
	game->config[SPR_BARREL].frame_count = 1;
	game->config[SPR_BARREL].is_animated = false;
	game->config[SPR_BARREL].is_directional = false;
	/* 缩放比例为 2.0，意味着木桶的高度和宽度只有墙高的一半 */
	/* 这样能让木桶看起来比树小，更符合道具的比例 */
	game->config[SPR_BARREL].v_div = 2.0f;
	game->config[SPR_BARREL].h_div = 2.0f;
	/* 垂直位移补偿：将木桶向地面方向“推”一段距离 */
	/* 因为缩小后的精灵默认可能悬浮在空中，v_move 确保它稳稳落地 */
	game->config[SPR_BARREL].v_move = 300.0f;
}

static void	init_dynamic_sprites(t_game *game)
{
	/* --- 火炬 (TORCH) 配置：循环动画 --- */
	/* 拥有 4 张连续贴图，形成火焰跳动的动画效果 */
	game->config[SPR_TORCH].frame_count = 4;
	game->config[SPR_TORCH].is_animated = true;
	/* 火焰是发光体，从哪个角度看都一样，所以不需要多方向贴图 */
	game->config[SPR_TORCH].is_directional = false;
	game->config[SPR_TORCH].v_div = 1.0f;
	game->config[SPR_TORCH].h_div = 1.0f;
	/* v_move 为负数，表示将精灵向上“提” */
	/* 这样火炬就会挂在墙壁的高处，而不是放在地板上 */
	game->config[SPR_TORCH].v_move = -320.0f;

	/* --- 怪物 (MONSTER) 配置：多方向视图 --- */
	/* 这里的 8 帧不是动画，而是 8 个观察方向（0°, 45°, 90°...）的贴图 */
	game->config[SPR_MONSTER].frame_count = 8;
	/* 设置为 false 是因为怪物平时可能只是站立，动画切换通常由 AI 状态触发 */
	game->config[SPR_MONSTER].is_animated = false;
	/* 核心设置：根据玩家观察怪物的角度，切换显示怪物的正面、侧面或背面 */
	game->config[SPR_MONSTER].is_directional = true;
	game->config[SPR_MONSTER].v_div = 2.0f;
	game->config[SPR_MONSTER].h_div = 2.0f;
	/* v_move 为正数，确保怪物稳稳地踩在地面上 */
	game->config[SPR_MONSTER].v_move = 300.0f;
}

void	init_sprite_texture(t_game *game)
{
	/* 1. 安全检查 */
	/* 确保游戏结构体和 MLX 实例已初始化，否则无法加载图像资源 */
	if (!game || !game->mlx)
		return ;

	/* 2. 定义静态属性 */
	/* 设置如树木、木桶等不随时间变化的精灵参数 (缩放、偏移、是否旋转) */
	init_static_sprites(game);

	/* 3. 定义动态属性 */
	/* 设置如火炬（动画）或怪物（多方向视角）的特殊渲染参数 */
	init_dynamic_sprites(game);

	/* 4. 加载二进制资源 */
	/* 真正从文件系统读取 .xpm 图像，并存储到 game->config[type].tex 中 */
	/* 这步完成后，每个精灵类型才拥有了可以显示在屏幕上的像素数据 */
	load_all_sprite_resources(game);
}
