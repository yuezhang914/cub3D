/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_sprites_resources_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:34:35 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:35:28 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：set_sprite_resource_path（static）
** 作用：为指定 sprite 类型的第 frame 帧，设置对应的 xpm 路径
**
** 参数：
**   game：写入 game->config[type].frames[frame].path
**   type：SPR_TREE / SPR_BARREL / SPR_TORCH / SPR_MONSTER
**   frame：第几帧（0..frame_count-1）
**
** 主要逻辑：
**   - 火把 torch：使用 t_paths[frame]
**   - 怪物 monster：使用 m_paths[frame]（8方向）
**   - 树/桶：固定单图路径
**
** 调用者：
**   load_all_sprite_resources()
*/
static void	set_sprite_resource_path(t_game *game, int type, int frame)
{
	static char	*t_paths[] = {"assets/sprites/torch_0.xpm",
		"assets/sprites/torch_1.xpm", "assets/sprites/torch_2.xpm",
		"assets/sprites/torch_3.xpm"};
	static char	*m_paths[] = {"assets/sprites/m_front.xpm",
		"assets/sprites/m_front_right.xpm", "assets/sprites/m_right.xpm",
		"assets/sprites/m_back_right.xpm", "assets/sprites/m_back.xpm",
		"assets/sprites/m_back_left.xpm", "assets/sprites/m_left.xpm",
		"assets/sprites/m_front_left.xpm"};

	if (type == SPR_TREE)
		game->config[type].frames[frame].path = "assets/sprites/tree.xpm";
	else if (type == SPR_BARREL)
		game->config[type].frames[frame].path = "assets/sprites/barrel.xpm";
	else if (type == SPR_TORCH)
		game->config[type].frames[frame].path = t_paths[frame];
	else if (type == SPR_MONSTER)
		game->config[type].frames[frame].path = m_paths[frame];
}

/*
** 函数名：load_all_sprite_resources
** 作用：加载所有 sprite 类型的所有帧贴图：
**      先为每个 type 分配 frames 数组，再逐帧设置 path 并 load_texture
**
** 参数：
**   game：提供 config[type].frame_count，并写入 config[type].frames[]
**
** 主要逻辑：
**   对每个 i in [0, SPR_COUNT):
**     1) 分配 frames：t_tex 数组，长度 frame_count
**     2) 对每一帧 f：
**        - set_sprite_resource_path 设置 path
**        - load_texture 读取 xpm，填 img_ptr/data/width/height/bpp/size_line
**
** 调用者：
**   init_sprite_texture()
*/
void	load_all_sprite_resources(t_game *game)
{
	int	i;
	int	f;

	i = -1;
	while (++i < SPR_COUNT)
	{
		game->config[i].frames = (t_tex *)track_malloc(game,
				sizeof(t_tex) * game->config[i].frame_count);
		f = -1;
		while (++f < game->config[i].frame_count)
		{
			set_sprite_resource_path(game, i, f);
			load_texture(game, &game->config[i].frames[f]);
		}
	}
}
