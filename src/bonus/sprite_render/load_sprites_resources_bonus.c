/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_sprites_resources_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:34:35 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:34:41 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void load_all_sprite_resources(t_game *game)
{
	int i;
	int f;

	i = -1;
	while (++i < SPR_COUNT)
	{
		/* 1. 为该类型的贴图数组分配内存 */
		/* 使用 track_malloc 确保程序退出时自动释放 */
		game->config[i].frames = (t_tex *)track_malloc(game,
				sizeof(t_tex) * game->config[i].frame_count);

		f = -1;
		while (++f < game->config[i].frame_count)
		{
			/* 2. 为每一帧指定路径 */
			set_sprite_resource_path(game, i, f);

			/* 3. 调用你的 load_texture 将 XPM 读入内存 */
			/* 确保 load_texture 内部填充了 width, height 和 data */
			load_texture(game, &game->config[i].frames[f]);

			/* 调试打印：确认资源是否到位 */
			//printf("Loaded: Type %d Frame %d -> %s\n", i, f, game->config[i].frames[f].path);
		}
	}
}
