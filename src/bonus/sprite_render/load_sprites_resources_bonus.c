/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_sprites_resources_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:34:35 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:34:41 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_sprite_resource_path(t_game *game, int type, int frame)
{
	/* 1. 定义动画/多方向视图的路径库 */
	/* t_paths 用于火炬的 4 帧动画序列 */
	static char *t_paths[] = {"assets/sprites/torch_0.xpm",
		"assets/sprites/torch_1.xpm", "assets/sprites/torch_2.xpm",
		"assets/sprites/torch_3.xpm"};
	/* m_paths 用于怪物的 8 方向视图（从正前开始顺时针排列） */
	static char *m_paths[] = {"assets/sprites/m_front.xpm",
		"assets/sprites/m_front_right.xpm", "assets/sprites/m_right.xpm",
		"assets/sprites/m_back_right.xpm", "assets/sprites/m_back.xpm",
		"assets/sprites/m_back_left.xpm", "assets/sprites/m_left.xpm",
		"assets/sprites/m_front_left.xpm"};

	/* 2. 根据类型分发路径 */
	if (type == SPR_TREE)
		game->config[type].frames[frame].path = "assets/sprites/tree.xpm";
	else if (type == SPR_BARREL)
		game->config[type].frames[frame].path = "assets/sprites/barrel.xpm";
	else if (type == SPR_TORCH)
		/* 根据 frame 索引从静态数组中取值 */
		game->config[type].frames[frame].path = t_paths[frame];
	else if (type == SPR_MONSTER)
		game->config[type].frames[frame].path = m_paths[frame];
}

void	load_all_sprite_resources(t_game *game)
{
	int	i;
	int	f;

	i = -1;
	/* 1. 遍历所有定义的精灵类型 (SPR_TREE, SPR_MONSTER 等) */
	while (++i < SPR_COUNT)
	{
		/* 2. 为该类型的帧数组分配内存 */
		/* 例如怪物有 8 帧，这里就分配 8 个 t_tex 结构体的空间 */
		game->config[i].frames = (t_tex *)track_malloc(game,
				sizeof(t_tex) * game->config[i].frame_count);
		f = -1;
		/* 3. 遍历每一帧进行资源加载 */
		while (++f < game->config[i].frame_count)
		{
			/* 关联路径字符串 */
			set_sprite_resource_path(game, i, f);
			/* 调用底层 mlx_xpm_file_to_image 等逻辑，将文件读入内存 */
			load_texture(game, &game->config[i].frames[f]);
		}
	}
}
