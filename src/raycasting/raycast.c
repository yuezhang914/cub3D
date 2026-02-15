/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:01:36 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/09 20:01:43 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 向图像缓冲区写入颜色
 * index 计算公式：(y * 宽度 + x) * 每个像素占用的字节数
 */
void put_pixel(int x, int y, int color, t_game *game)
{
	// 边界安全检查，防止非法内存写入导致 Segfault
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return;
	int index = y * game->size_line + x * (game->bpp / 8);
	// 按照小端序 (B-G-R) 写入颜色分量
	game->data[index] = color & 0xFF;			  // Blue
	game->data[index + 1] = (color >> 8) & 0xFF;  // Green
	game->data[index + 2] = (color >> 16) & 0xFF; // Red
	game->data[index + 3] = 0xFF;				  // Alpha (不透明)
}

/**
 * 辅助函数：将像素画到小地图区域（带 20 像素的边距偏移）
 */
static void put_mini_pixel(t_game *game, int x, int y, int color)
{
	// 确保不超出小地图定义的 200x200 区域
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return;
	// 这里的 20 是在主窗口左上角的起始偏移量
	put_pixel(x + 20, y + 20, color, game);
}

/**
 * 绘制玩家标识和方向线
 */
static void draw_mini_player(t_game *game)
{
	int center_x = MINI_WIDTH / 2;
	int center_y = MINI_HEIGHT / 2;
	int i;

	// 1. 绘制一个 5x5 的红色像素块代表玩家
	for (int y = -2; y <= 2; y++)
	{
		for (int x = -2; x <= 2; x++)
			put_mini_pixel(game, center_x + x, center_y + y, 0xFF0000);
	}
	// 2. 绘制一条绿色的方向线（长度为 12 像素）
	for (i = 0; i < 12; i++)
	{
		int line_x = center_x + (int)(cos(game->player.angle) * i);
		int line_y = center_y + (int)(sin(game->player.angle) * i);
		put_mini_pixel(game, line_x, line_y, 0x00FF00);
	}
}

/**
 * 核心渲染函数：以玩家为中心的动态滚动小地图
 */
void render_minimap(t_game *game)
{
	int x;
	int y;
	float map_x;
	float map_y;
	int color;

	y = 0;
	while (y < MINI_HEIGHT)
	{
		x = 0;
		while (x < MINI_WIDTH)
		{
			// 利用你定义的 pix_per_unit 进行坐标反推
			// 原理：玩家坐标 + (当前像素与中心的距离 / 比例尺)
			map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
			map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;

			// 1. 安全检查：防止地图数组越界（Segfault 杀手）
			if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
				color = 0x1A1A1A; // 地图外的虚无（深灰）
			// 2. 墙壁检测
			else if (game->map[(int)map_y][(int)map_x] == '1')
				color = 0xFFFFFF; // 墙壁（白色）
			// 3. 空地
			else
				color = 0x333333; // 地面（浅灰）

			put_mini_pixel(game, x, y, color);
			x++;
		}
		y++;
	}
	// 在背景画完后，最后覆盖玩家位置
	draw_mini_player(game);
}
/**
 * 渲染屏幕的一列像素：天花板 -> 墙壁 -> 地板
 */
/**
 * 渲染屏幕的一列像素 (符合 Norm 规范)
 * 作用：利用封装后的 v 结构体获取渲染区间和纹理信息，
 * 依次在屏幕第 x 列绘制天花板、经过缩放处理的墙体纹理以及地板。
 * 参数：v - 包含所有列渲染所需变量的结构体；game - 游戏主结构体。
 */
void render_column(t_render_vars v, t_game *game)
{
	int y;
	float step;
	float tex_pos;
	int color;

	// 1. 绘制天花板
	y = -1;
	while (++y < (v.start < 0 ? 0 : v.start))
		put_pixel(v.x, y, game->ceiling_color, game);

	// 2. 绘制墙体
	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = (v.start < 0 ? (0 - v.start) : 0) * step;
	y = (v.start < 0) ? 0 : v.start;
	while (y <= (v.end >= HEIGHT ? HEIGHT - 1 : v.end))
	{
		color = *(int *)(v.tex->data + ((int)tex_pos % v.tex->height *
											v.tex->size_line +
										v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}

	// 3. 绘制地板
	y--;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}
/**
 * 核心射线投射逻辑 (DDA 算法)
 * @param i: 当前渲染的是屏幕的第几列
 */
/**
 * 执行 DDA 算法并渲染单列像素
 * @param r_dir_x 射线的 X 方向向量
 * @param r_dir_y 射线的 Y 方向向量
 * @param i       当前渲染的屏幕列索引 (0 到 WIDTH-1)
 */
void draw_line(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_player *p = &game->player;

	// --- 1. DDA 初始化 ---
	int map_x = (int)p->x;
	int map_y = (int)p->y;

	// 计算每移动一个网格所需的距离 (delta_dist)
	float delta_x = (r_dir_x == 0) ? 1e30 : fabsf(1.0f / r_dir_x);
	float delta_y = (r_dir_y == 0) ? 1e30 : fabsf(1.0f / r_dir_y);

	int step_x, step_y;
	float side_x, side_y;

	// 计算步进方向和初始边距距离
	if (r_dir_x < 0)
	{
		step_x = -1;
		side_x = (p->x - map_x) * delta_x;
	}
	else
	{
		step_x = 1;
		side_x = (map_x + 1.0f - p->x) * delta_x;
	}
	if (r_dir_y < 0)
	{
		step_y = -1;
		side_y = (p->y - map_y) * delta_y;
	}
	else
	{
		step_y = 1;
		side_y = (map_y + 1.0f - p->y) * delta_y;
	}

	// --- 2. DDA 循环：寻找墙壁 ---
	int side = 0; // 0: 东西墙(左右), 1: 南北墙(上下)
	while (1)
	{
		if (side_x < side_y)
		{
			side_x += delta_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_y += delta_y;
			map_y += step_y;
			side = 1;
		}
		// 安全检查：如果跳出地图边界则停止
		if (map_x < 0 || map_x >= game->map_w || map_y < 0 || map_y >= game->map_h)
			return;
		// 碰到墙壁 ('1')
		if (game->map[map_y][map_x] == '1')
			break;
	}

	// --- 3. 计算垂直墙距 (修正鱼眼的关键) ---
	// 直接使用向量投影计算，无需 cos 修正
	float perp_dist;
	if (side == 0)
		perp_dist = side_x - delta_x;
	else
		perp_dist = side_y - delta_y;

	// 防止除以 0 导致崩溃
	if (perp_dist < 0.1f)
		perp_dist = 0.1f;

	// --- 4. 计算渲染高度 ---
	int line_h = (int)(HEIGHT / perp_dist);
	int start = (HEIGHT / 2) - (line_h / 2);
	int end = (HEIGHT / 2) + (line_h / 2);

	// --- 5. 选择纹理 ---
	t_tex *tex;
	if (side == 0)
		tex = (r_dir_x > 0) ? &game->east : &game->west;
	else
		tex = (r_dir_y > 0) ? &game->south : &game->north;

	// --- 6. 计算纹理 X 坐标 (tex_x) ---
	float wall_hit; // 墙面上的精确碰撞点 (0.0 到 1.0)
	if (side == 0)
		wall_hit = p->y + perp_dist * r_dir_y;
	else
		wall_hit = p->x + perp_dist * r_dir_x;
	wall_hit -= floor(wall_hit);

	int tex_x = (int)(wall_hit * (float)tex->width);
	// 修正贴图方向，防止文字反向
	if ((side == 0 && r_dir_x < 0) || (side == 1 && r_dir_y > 0))
		tex_x = tex->width - tex_x - 1;

	// --- 7. 调用渲染函数绘制这一列 ---
	t_render_vars v;
	v.x = i;
	v.start = start;
	v.end = end;
	v.line_h = line_h;
	v.tex_x = tex_x;
	v.tex = tex;
	render_column(v, game);
}

/**
 * MLX 渲染循环：处理清屏、绘图、位置更新
 */
int draw_loop(t_game *game)
{
	t_player *player = &game->player;

	// 1. 获取玩家方向向量 (单位向量)
	float dir_x = cos(player->angle);
	float dir_y = sin(player->angle);

	// 2. 使用 focal_length 计算视平面向量 (相机平面)
	// 注意：focal_length 是总宽度，所以两边延伸各占一半
	float plane_x = -dir_y * (game->focal_length / 2.0f);
	float plane_y = dir_x * (game->focal_length / 2.0f);

	int i = 0;

	while (i < WIDTH)
	{
		// 3. 计算射线方向 (x 范围从 -1 到 1)
		float camera_x = 2.0f * i / (float)WIDTH - 1.0f;
		float ray_dir_x = dir_x + plane_x * camera_x;
		float ray_dir_y = dir_y + plane_y * camera_x;

		// 4. 调用射线投射 (建议直接传 ray_dir 而不是角度，效率更高)
		draw_line(game, ray_dir_x, ray_dir_y, i);
		i++;
	}

	// 5. 更新位置与小地图
	move_player(player, game);
	render_minimap(game); // 使用你定义的 pix_per_unit 绘制

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
