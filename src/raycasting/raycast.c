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
 * 获取地图数据
 * '1' 代表墙壁，'0' 代表走廊，NULL 结尾用于遍历
 */
char **get_map(void)
{
	char **map = malloc(sizeof(char *) * 11);
	map[0] = "111111111111111";
	map[1] = "100000000000001";
	map[2] = "100000000000001";
	map[3] = "100000000000001";
	map[4] = "100000000000001";
	map[5] = "100000001000001"; // 地图中间的一个孤立柱子
	map[6] = "100000000000001";
	map[7] = "100000000000001";
	map[8] = "100000000000001";
	map[9] = "111111111111111";
	map[10] = NULL;
	return (map);
}

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
 * 在 2D 小地图模式下绘制地图方块
 */
int draw_square(int x, int y, int size, int color, t_game *game)
{
	int i, j;

	for (i = 0; i < size; i++)
		put_pixel(x + i, y, color, game); // 上边
	for (j = 0; j < size; j++)
		put_pixel(x + size - 1, y + j, color, game); // 右边
	for (i = 0; i < size; i++)
		put_pixel(x + i, y + size - 1, color, game); // 下边
	for (j = 0; j < size; j++)
		put_pixel(x, y + j, color, game); // 左边
	return 0;
}

/**
 * 每一帧渲染前清空图像（将所有像素设为黑色 0）
 */
void clear_img(t_game *game)
{
	int i = 0;
	while (i < HEIGHT)
	{
		int j = 0;
		while (j < WIDTH)
		{
			put_pixel(j, i, 0, game);
			j++;
		}
		i++;
	}
}

/**
 * 遍历地图数组并在 2D 小地图上绘制所有的墙 ('1')
 */
void draw_map(t_game *game)
{
	char **map = game->map;
	int i = 0;

	while (map[i])
	{
		int j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '1')
				draw_square(j * BLOCK, i * BLOCK, BLOCK, 0xFF, game);
			j++;
		}
		i++;
	}
}

/**
 * 碰撞检测函数：判断坐标 (px, py) 是否位于墙壁内
 */
bool touch(float px, float py, t_game *game)
{
	if (px < 0 || py < 0 || px >= WIDTH || py >= HEIGHT)
		return (true);

	// 将像素坐标转换为地图数组索引
	int x = (int)px / BLOCK;
	int y = (int)py / BLOCK;

	// 防止数组索引越界
	if (y < 0 || y >= 10 || x < 0 || x >= 15)
		return (true);

	return (game->map[y][x] == '1');
}

/**
 * 渲染屏幕的一列像素：天花板 -> 墙壁 -> 地板
 */
void render_column(int x, int start, int end, int tex_x, t_tex *tex, t_game *game)
{
	// 1. 计算纹理步进（每个像素对应多少纹理像素）
	float step = 1.0 * tex->height / (end - start);

	// 2. 计算纹理初始位置 (tex_pos)
	// 如果 start < 0，说明墙顶在屏幕上方，我们需要跳过那部分纹理
	float tex_pos = 0;
	if (start < 0)
	{
		tex_pos = -start * step; // 补偿：将纹理坐标向下挪
	}

	// 3. 限制循环范围，防止越界写入
	int draw_start = (start < 0) ? 0 : start;
	int draw_end = (end > HEIGHT) ? HEIGHT : end;

	for (int y = 0; y < HEIGHT; y++)
	{
		if (y < draw_start)
		{
			// 渲染天花板
			put_pixel(x, y, game->ceiling_color, game);
		}
		else if (y >= draw_start && y < draw_end)
		{
			// 渲染墙壁
			int tex_y = (int)tex_pos % tex->height;
			tex_pos += step;

			int color = tex->addr[tex_y * tex->width + tex_x];
			put_pixel(x, y, color, game);
		}
		else
		{
			// 渲染地板
			put_pixel(x, y, game->floor_color, game);
		}
	}
}
/**
 * 核心射线投射逻辑 (DDA 算法)
 * @param i: 当前渲染的是屏幕的第几列
 */
void draw_line(t_player *player, t_game *game, float ray_angle, int i)
{
	// --- 1. 初始化 DDA 变量 ---
	int map_x = (int)(player->x / BLOCK); // 射线当前所在的地图格子坐标
	int map_y = (int)(player->y / BLOCK);

	float ray_dir_x = cos(ray_angle); // 射线方向向量
	float ray_dir_y = sin(ray_angle);

	// delta_dist: 射线跨越一个完整网格线所需的斜边长度
	float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabsf(1 / ray_dir_x);
	float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabsf(1 / ray_dir_y);

	float side_dist_x; // 射线到下一条网格线的初始长度
	float side_dist_y;
	int step_x; // 步进方向 (1 或 -1)
	int step_y;

	// --- 2. 计算步进方向和 side_dist 的“第一跳”起始距离 ---
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (player->x / BLOCK - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - player->x / BLOCK) * delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (player->y / BLOCK - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - player->y / BLOCK) * delta_dist_y;
	}

	// --- 3. DDA 循环：跳格子直到撞墙 ---
	int side = 0; // 记录撞击的是垂直墙(0)还是水平墙(1)
	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (map_y < 0 || map_y >= 10 || map_x < 0 || map_x >= 15)
			break;
		if (game->map[map_y][map_x] == '1')
			break;
	}

	// --- 4. 距离修正 (核心：计算垂直投影距离以消除鱼眼效应) ---
	float perp_wall_dist;
	if (side == 0)
		perp_wall_dist = side_dist_x - delta_dist_x;
	else
		perp_wall_dist = side_dist_y - delta_dist_y;

	// 进一步修正：将距离投影到玩家视角正前方
	float angle_diff = ray_angle - player->angle;
	float corrected_dist = perp_wall_dist * cos(angle_diff);
	float dist_pixels = corrected_dist * BLOCK;

	// --- 5. 计算墙壁高度并进行垂直居中 ---
	if (dist_pixels < 0.1f)
		dist_pixels = 0.1f;
	float height = (BLOCK * HEIGHT) / dist_pixels;

	int wall_start = (HEIGHT / 2) - (height / 2); // 墙顶位置
	int wall_end = (HEIGHT / 2) + (height / 2);	  // 墙底位置

	// 在 draw_line 函数中，计算完距离后加入：
	t_tex *tex;

	if (side == 0) // 撞击垂直墙（左右方向）
	{
		if (ray_dir_x > 0) // 射线向右，撞到的是墙的西面（显示东贴图）
			tex = &game->east;
		else // 射线向左，撞到的是墙的东面（显示西贴图）
			tex = &game->west;
	}
	else // 撞击水平墙（上下方向）
	{
		if (ray_dir_y > 0) // 射线向下，撞到的是墙的北面（显示南贴图）
			tex = &game->south;
		else // 射线向上，撞到的是墙的南面（显示北贴图）
			tex = &game->north;
	}

	float wall_x;
	if (side == 0)
		wall_x = player->y / BLOCK + perp_wall_dist * ray_dir_y;
	else
		wall_x = player->x / BLOCK + perp_wall_dist * ray_dir_x;
	wall_x -= floor(wall_x);

	int tex_x = (int)(wall_x * (float)tex->width);

	// 修正镜像，确保你绕着柱子走的时候贴图不会“倒着走”
	if ((side == 0 && ray_dir_x < 0) || (side == 1 && ray_dir_y > 0))
		tex_x = tex->width - tex_x - 1;

	// 渲染天花板、墙壁、地板
	render_column(i, wall_start, wall_end, tex_x, tex, game);
}

/**
 * MLX 渲染循环：处理清屏、绘图、位置更新
 */
int draw_loop(t_game *game)
{
	t_player *player = &game->player;
	clear_img(game);

	if (DEBUG)
	{
		draw_square(player->x, player->y, 10, 0x00FF00, game); // 画玩家点
		draw_map(game);										   // 画 2D 地图
	}

	// 发射射线扫描全屏，FOV 设为 60 度 (PI / 3)
	float fraction = (PI / 3) / WIDTH;
	float start_angle = player->angle - (PI / 6);

	int i = 0;
	while (i < WIDTH)
	{
		draw_line(player, game, start_angle, i);
		start_angle += fraction;
		i++;
	}

	move_player(player, game); // 根据按键更新玩家坐标
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return 0;
}
