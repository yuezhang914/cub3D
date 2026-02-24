/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_ray_fov.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:49:57 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 14:50:01 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_mini_vision_line(t_game *game)
{
	t_int_xy	c;     /* 小地图中心点 (玩家位置) */
	t_coords	ray;   /* 射线当前的像素坐标 */
	float		dist;  /* 射线延伸的距离 */
	int			color; /* 用于探测碰撞的颜色值 */

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;
	dist = 0;
	/* 1. 从中心向外延伸，最大长度为小地图半径 */
	while (dist < MINI_WIDTH / 2)
	{
		/* 2. 利用三角函数计算射线上每一个点的像素位置 */
		ray.x = c.x + cos(game->player.angle) * dist;
		ray.y = c.y + sin(game->player.angle) * dist;

		/* 3. 碰撞检测：采样该点的颜色 */
		color = get_mini_color(game, (int)ray.x, (int)ray.y);
		/* 如果遇到墙壁 (0xFFFFFF) 或地图边界 (0x1A1A1A)，停止绘制 */
		if (color == 0xFFFFFF || color == 0x1A1A1A)
			break ;

		/* 4. 绘制像素：使用亮绿色 (0x55FF55) */
		put_mini_pixel(game, (int)ray.x, (int)ray.y, 0x55FF55);
		dist += 1.0f;
	}
}

/* 绘制 FOV 中的单条细微射线 */
static void	draw_vision_ray(t_game *game, float angle)
{
	t_int_xy	c;
	float		dist;
	int			rx;
	int			ry;

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;
	dist = 0;
	/* FOV 射线较短（仅30像素），起到半透明雷达的效果 */
	while (dist < 30)
	{
		rx = c.x + cos(angle) * dist;
		ry = c.y + sin(angle) * dist;
		/* 碰到墙壁即停止 */
		if (get_mini_color(game, rx, ry) == 0xFFFFFF)
			break ;
		/* 0x3300FF00 带有一个 Alpha 通道（取决于 put_pixel 实现）或特定的淡绿色 */
		put_mini_pixel(game, rx, ry, 0x3300FF00);
		dist += 1.0f;
	}
}

/* 绘制整个视野扇形 */
void	draw_mini_fov(t_game *game)
{
	float	start_angle;
	float	end_angle;
	float	curr_angle;

	/* 1. 计算视野范围：中心角度 +/- 30度 (PI/6) */
	start_angle = game->player.angle - (PI / 6);
	end_angle = game->player.angle + (PI / 6);
	curr_angle = start_angle;

	/* 2. 步进绘制：每隔 2 度 (PI/90) 发射一条探测射线 */
	while (curr_angle <= end_angle)
	{
		draw_vision_ray(game, curr_angle);
		curr_angle += (PI / 90);
	}
}

