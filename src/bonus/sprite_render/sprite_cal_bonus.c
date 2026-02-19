/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_cal_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:33:41 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 11:33:43 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void set_sprite_resource_path(t_game *game, int type, int frame)
{
	t_tex *tex = &game->config[type].frames[frame];

	// 注意：路径字符串必须与你 ls 出来的结果完全一致
	if (type == SPR_TREE)
		tex->path = "assets/sprites/tree.xpm";
	else if (type == SPR_BARREL)
		tex->path = "assets/sprites/barrel.xpm";
	else if (type == SPR_TORCH)
	{
		// 你的目录里有 torch_0 到 torch_3
		if (frame == 0)
			tex->path = "assets/sprites/torch_0.xpm";
		else if (frame == 1)
			tex->path = "assets/sprites/torch_1.xpm";
		else if (frame == 2)
			tex->path = "assets/sprites/torch_2.xpm";
		else
			tex->path = "assets/sprites/torch_3.xpm";
	}
	else if (type == SPR_MONSTER)
	{
		// 这里的顺序必须对应你的 get_sprite_dir_index 逻辑
		char *m_paths[] = {
			"assets/sprites/m_front.xpm",		// 0
			"assets/sprites/m_front_right.xpm", // 1
			"assets/sprites/m_right.xpm",		// 2
			"assets/sprites/m_back_right.xpm",	// 3
			"assets/sprites/m_back.xpm",		// 4
			"assets/sprites/m_back_left.xpm",	// 5
			"assets/sprites/m_left.xpm",		// 6
			"assets/sprites/m_front_left.xpm"	// 7
		};
		tex->path = m_paths[frame];
	}
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
			printf("Loaded: Type %d Frame %d -> %s\n", i, f, game->config[i].frames[f].path);
		}
	}
}

void init_sprite_texture(t_game *game)
{
	if (!game || !game->mlx)
		return;

	// --- [1] 树：静态单帧 ---
	game->config[SPR_TREE].frame_count = 1;
	game->config[SPR_TREE].is_animated = false;
	game->config[SPR_TREE].is_directional = false;
	game->config[SPR_TREE].v_move = 0.0f; // 树长在地上

	// --- [2] 动画火炬：4 帧循环 ---
	game->config[SPR_TORCH].frame_count = 4;
	game->config[SPR_TORCH].is_animated = true;
	game->config[SPR_TORCH].is_directional = false;
	game->config[SPR_TORCH].v_move = -200.0f; // 火炬通常挂在墙上，向上偏移一点

	// --- [3] 桶：静态单帧 (矮胖型) ---
	game->config[SPR_BARREL].frame_count = 1;
	game->config[SPR_BARREL].is_animated = false;
	game->config[SPR_BARREL].is_directional = false;

	// 缩放系数：值越大，精灵越小
	game->config[SPR_BARREL].v_div = 2.0f; // 高度缩小一倍
	game->config[SPR_BARREL].h_div = 2.0f; // 宽度缩小一倍

	// 垂直位移：因为高度变小了，中心点上移，所以需要增加 v_move 将其推回地面
	// 公式参考：v_move = vertical_offset / transform_y
	game->config[SPR_BARREL].v_move = 300.0f;

	// --- [4] 怪物：8 方向立体 ---
	game->config[SPR_MONSTER].frame_count = 8;
	game->config[SPR_MONSTER].is_animated = false;
	game->config[SPR_MONSTER].is_directional = true;
	game->config[SPR_MONSTER].v_move = 0.0f;

	// 执行所有资源的分配与加载
	load_all_sprite_resources(game);
}

void collect_sprites(t_game *game)
{
	int i;
	int j;
	int count;

	count = 0;

	/* 第一遍：数数 */
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (ft_strchr("TBCM", game->map[i][j]))
				count++;
		}
	}

	game->sprs.num = count;

	if (count == 0)
		return;

	game->sprs.list = track_malloc(game, sizeof(t_sprite) * count);

	/* 第二遍：填充数据 */
	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (ft_strchr("TBCM", game->map[i][j]))
			{
				game->sprs.list[count].x = j + 0.5f;
				game->sprs.list[count].y = i + 0.5f;

				if (game->map[i][j] == 'T')
					game->sprs.list[count].type = SPR_TREE;
				else if (game->map[i][j] == 'B')
					game->sprs.list[count].type = SPR_BARREL;
				else if (game->map[i][j] == 'C')
					game->sprs.list[count].type = SPR_TORCH;
				else if (game->map[i][j] == 'M')
					game->sprs.list[count].type = SPR_MONSTER;

				game->map[i][j] = '0';
				count++;
			}
		}
	}
}

/**
 * @brief 计算每个精灵到玩家当前位置的距离平方
 * @param game 游戏主结构体
 */
void calculate_sprite_distance(t_game *game)
{
	int i;
	float px;
	float py;

	i = 0;
	px = game->player.x;
	py = game->player.y;
	while (i < game->sprs.num)
	{
		// 距离平方公式: (x1 - x2)^2 + (y1 - y2)^2
		game->sprs.list[i].dist = ((px - game->sprs.list[i].x) * (px - game->sprs.list[i].x) + (py - game->sprs.list[i].y) * (py - game->sprs.list[i].y));
		i++;
	}
}

/*
** 计算每个精灵到玩家的距离平方
*/
void update_sprite_distances(t_game *game)
{
	int i;

	i = 0;
	while (i < game->sprs.num)
	{
		// 计算坐标差
		float dx = game->player.x - game->sprs.list[i].x;
		float dy = game->player.y - game->sprs.list[i].y;

		// 存储距离平方 (dx^2 + dy^2)
		game->sprs.list[i].dist = (dx * dx) + (dy * dy);
		i++;
	}
}

/*
** 执行排序 (画家算法：从远到近)
*/
void sort_sprites(t_game *game)
{
	int i;
	int j;
	t_sprite temp;

	if (game->sprs.num < 2)
		return;

	// 1. 先更新最新距离
	update_sprite_distances(game);

	// 2. 冒泡排序：将距离远的排在数组前面
	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			// 如果后一个比前一个更远，则交换（保证数组是从远到近）
			if (game->sprs.list[j].dist < game->sprs.list[j + 1].dist)
			{
				temp = game->sprs.list[j];
				game->sprs.list[j] = game->sprs.list[j + 1];
				game->sprs.list[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}