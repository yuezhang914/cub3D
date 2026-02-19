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

	if (type == SPR_TREE)
		tex->path = "assets/sprites/tree.xpm";
	else if (type == SPR_BARREL)
		tex->path = "assets/sprites/barrel.xpm";
	else if (type == SPR_TORCH)
	{
		/* 假设你有 torch_0.xpm 到 torch_3.xpm */
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
		/* 8 方向怪物的路径映射 */
		char *m_paths[] = {
			"assets/sprites/m_front.xpm", "assets/sprites/m_front_right.xpm",
			"assets/sprites/m_right.xpm", "assets/sprites/m_back_right.xpm",
			"assets/sprites/m_back.xpm", "assets/sprites/m_back_left.xpm",
			"assets/sprites/m_left.xpm", "assets/sprites/m_front_left.xpm"};
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
			// printf("Loaded: Type %d Frame %d -> %s\n", i, f, game->config[i].frames[f].path);
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
	game->config[SPR_BARREL].v_move = 0.0f; // 桶踩在地上

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
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (ft_strchr("TBCM", game->map[i][j]))
			{
				// 设置坐标（加 0.5 放在格子中心）
				game->sprs.list[count].x = (float)j + 0.5f;
				game->sprs.list[count].y = (float)i + 0.5f;

				// 根据地图字符设置对应的 type
				if (game->map[i][j] == 'T')
					game->sprs.list[count].type = SPR_TREE;
				else if (game->map[i][j] == 'B')
					game->sprs.list[count].type = SPR_BARREL;
				else if (game->map[i][j] == 'C')
					game->sprs.list[count].type = SPR_TORCH;
				else if (game->map[i][j] == 'M')
					game->sprs.list[count].type = SPR_MONSTER;

				// 初始化状态
				game->sprs.list[count].cur_frame = 0;
				game->sprs.list[count].last_update = 0;

				// ⚠️ 关键：将地图抹除为 '0'，否则 DDA 会把精灵当成不透明的墙
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

/**
 * @brief 对精灵数组进行降序排序（从远到近）。
 * * 逻辑：Painter's Algorithm (画家算法)。
 * 我们必须先绘制最远的精灵，这样近处的精灵才能正确覆盖在其上。
 */
void sort_sprites(t_game *game)
{
	int i;
	int j;
	t_sprite temp;

	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			// 如果后一个精灵比前一个远，则交换位置
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
