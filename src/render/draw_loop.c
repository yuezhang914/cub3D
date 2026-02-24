/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_loop.c                                            :+:      :+:    : */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:06:22 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/18 01:45:30 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	setup_camera(t_game *game, float dir[2], float plane[2])
{
	/* 1. 计算玩家当前正对着的方向向量 (视线中心) */
	/* dir[0] 是 X 分量，dir[1] 是 Y 分量 */
	dir[0] = cos(game->player.angle);
	dir[1] = sin(game->player.angle);

	/* 2. 计算垂直于视线的相机平面（摄像机焦距/视野宽度） */
	/* 我们利用垂直向量原理：(x, y) 的垂直向量是 (-y, x) */
	/* game->focal_length 通常对应 FOV，决定了平面的长度 */
	plane[0] = -dir[1] * (game->focal_length / 2.0f);
	plane[1] = dir[0] * (game->focal_length / 2.0f);
}

void	draw_loop(t_game *game)
{
	float		dir[2];      /* 玩家的正前方视线向量 (方向向量) */
	float		plane[2];    /* 垂直于视线的相机平面向量 (决定 FOV) */
	int			x;           /* 当前正在渲染的屏幕列索引 (0 到 WIDTH - 1) */
	float		cam_x;       /* 当前列在相机平面上的横坐标 (-1 到 1) */
	t_coords	ray_dir;     /* 最终计算出的单条射线方向向量 */

	/* 1. 初始化相机：计算玩家当前的视线 dir 和垂直的相机平面 plane */
	setup_camera(game, dir, plane);
	x = 0;

	/* 2. 逐列扫描：遍历屏幕宽度上的每一像素列 */
	while (x < WIDTH)
	{
		/* 3. 映射归一化坐标：将像素坐标 x 转换到 [-1, 1] 空间 */
		/* 当 x = 0 (最左), cam_x = -1 */
		/* 当 x = WIDTH/2 (中间), cam_x = 0 */
		/* 当 x = WIDTH-1 (最右), cam_x = 1 */
		cam_x = 2.0f * x / (float)WIDTH - 1.0f;

		/* 4. 合成射线向量：视线向量 + (相机平面分量 * 权重) */
		/* 这是线性组合：ray = dir + plane * cam_x */
		/* 这样产生了一束以玩家为顶点、向前方扇形展开的射线束 */
		ray_dir.x = dir[0] + plane[0] * cam_x;
		ray_dir.y = dir[1] + plane[1] * cam_x;

		/* 5. 执行渲染：为这一列像素进行 DDA 计算、纹理采样并绘图 */
		render_column(game, ray_dir.x, ray_dir.y, x);
		x++;
	}
}

void	clear_image(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}
