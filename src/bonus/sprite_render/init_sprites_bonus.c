/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:12 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:35:11 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：init_static_sprites（static）
** 作用：初始化“静态精灵”的配置（不动画、不方向切换）
**      这里包括：树（TREE）和桶（BARREL）
**
** 参数：
**   game：写入 game->config[SPR_TREE] 和 game->config[SPR_BARREL]
**
** 主要逻辑（每个字段意义）：
**   - frame_count：有几帧贴图
**   - is_animated：是否需要切帧（树/桶都 false）
**   - is_directional：是否按 8方向选贴图（树/桶都 false）
**   - v_div/h_div：缩放除数（越大 sprite 看起来越小）
**   - v_move：上下偏移（像素/相机深度相关量，渲染时会除以距离）
**
** 调用者：
**   init_sprite_texture()
*/
static void	init_static_sprites(t_game *game)
{
	game->config[SPR_TREE].frame_count = 1;
	game->config[SPR_TREE].is_animated = false;
	game->config[SPR_TREE].is_directional = false;
	game->config[SPR_TREE].v_div = 1.0f;
	game->config[SPR_TREE].h_div = 1.0f;
	game->config[SPR_TREE].v_move = 0.0f;

	game->config[SPR_BARREL].frame_count = 1;
	game->config[SPR_BARREL].is_animated = false;
	game->config[SPR_BARREL].is_directional = false;
	game->config[SPR_BARREL].v_div = 2.0f;
	game->config[SPR_BARREL].h_div = 2.0f;
	game->config[SPR_BARREL].v_move = 300.0f;
}

/*
** 函数名：init_dynamic_sprites（static）
** 作用：初始化“动态/复杂精灵”的配置
**      这里包括：火把（TORCH）与怪物（MONSTER）
**
** 参数：
**   game：写入对应 config
**
** 主要逻辑：
**   - 火把：
**     frame_count=4（torch_0..3）
**     is_animated=true（理论上可以切帧）
**     is_directional=false（不看方向）
**     v_move=-320（往上抬）
**
**   - 怪物：
**     frame_count=8（8方向贴图）
**     is_animated=false（这里不切帧）
**     is_directional=true（按方向选贴图）
**
** 调用者：
**   init_sprite_texture()
*/
static void	init_dynamic_sprites(t_game *game)
{
	game->config[SPR_TORCH].frame_count = 4;
	game->config[SPR_TORCH].is_animated = true;
	game->config[SPR_TORCH].is_directional = false;
	game->config[SPR_TORCH].v_div = 1.0f;
	game->config[SPR_TORCH].h_div = 1.0f;
	game->config[SPR_TORCH].v_move = -320.0f;

	game->config[SPR_MONSTER].frame_count = 8;
	game->config[SPR_MONSTER].is_animated = false;
	game->config[SPR_MONSTER].is_directional = true;
	game->config[SPR_MONSTER].v_div = 2.0f;
	game->config[SPR_MONSTER].h_div = 2.0f;
	game->config[SPR_MONSTER].v_move = 300.0f;
}

/*
** 函数名：init_sprite_texture
** 作用：sprite 系统初始化入口：先初始化 config（规则），再加载所有 sprite 贴图资源
**
** 参数：
**   game：需要 game->mlx（因为加载 xpm 需要 mlx 句柄）
**
** 主要逻辑：
**   1) 空指针保护：game 或 game->mlx 为空直接 return
**   2) init_static_sprites + init_dynamic_sprites：填 config
**   3) load_all_sprite_resources：为每个 type 分配 frames 数组并 load_texture
**
** 调用者：
**   一般在 bonus setup 阶段（mlx_init 之后）调用
**   例如 setup_mlx()（BONUS）里会 init_sprite_texture(game)
*/
void	init_sprite_texture(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	init_static_sprites(game);
	init_dynamic_sprites(game);
	load_all_sprite_resources(game);
}
