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
 * 渲染屏幕的一列像素：天花板 -> 墙壁 -> 地板
 */
void render_column(int x, int start, int end, int tex_x, t_tex *tex, t_game *game)
{
	// 保存原始高度用于计算步进，避免受屏幕边界剪裁的影响
	float raw_height = (float)(end - start);
	if (raw_height <= 0)
		raw_height = 1.0f;

	float step = (float)tex->height / raw_height;

	// 计算纹理起始点：如果墙很高，我们需要跳过上面不在屏幕内的贴图部分
	float tex_pos = 0;
	if (start < 0)
		tex_pos = (0 - start) * step;

	// 确定在屏幕上实际绘画的像素范围
	int draw_y = (start < 0) ? 0 : start;
	int draw_end = (end >= HEIGHT) ? HEIGHT - 1 : end;

	for (int y = 0; y < HEIGHT; y++)
	{
		if (y < draw_y)
			put_pixel(x, y, game->ceiling_color, game);
		else if (y <= draw_end)
		{
			int tex_y = (int)tex_pos & (tex->height - 1); // 位运算防溢出（要求贴图高度是2的幂）
			// 或者使用安全取模：int tex_y = (int)tex_pos % tex->height;

			int color = tex->addr[tex_y * tex->width + tex_x];
			put_pixel(x, y, color, game);
			tex_pos += step;
		}
		else
			put_pixel(x, y, game->floor_color, game);
	}
}
/**
 * 核心射线投射逻辑 (DDA 算法)
 * @param i: 当前渲染的是屏幕的第几列
 */
void draw_line(t_player *player, t_game *game, float ray_angle, int i)
{
	// --- 1. 初始化 ---
	int cur_map_x = (int)player->x;
	int cur_map_y = (int)player->y;
	float ray_dir_x = cos(ray_angle);
	float ray_dir_y = sin(ray_angle);

	float delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabsf(1 / ray_dir_x);
	float delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabsf(1 / ray_dir_y);

	float side_dist_x;
	float side_dist_y;
	int step_x;
	int step_y;

	// --- 2. 计算起始距离 ---
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (player->x - (float)cur_map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = ((float)cur_map_x + 1.0f - player->x) * delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (player->y - (float)cur_map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = ((float)cur_map_y + 1.0f - player->y) * delta_dist_y;
	}

	// --- 3. DDA 循环 ---
	int side = 0;
	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			cur_map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			cur_map_y += step_y;
			side = 1;
		}
		if (cur_map_y < 0 || cur_map_y >= game->map_h || cur_map_x < 0 || cur_map_x >= game->map_w)
			break;
		if (game->map[cur_map_y][cur_map_x] == '1')
			break;
	}

	// --- 4. 投影计算 ---
	float perp_wall_dist;
	if (side == 0)
		perp_wall_dist = side_dist_x - delta_dist_x;
	else
		perp_wall_dist = side_dist_y - delta_dist_y;

	if (perp_wall_dist < 0.0001f)
		perp_wall_dist = 0.0001f;

	float height = (float)HEIGHT / perp_wall_dist;
	int wall_start = (HEIGHT / 2) - (int)(height / 2);
	int wall_end = (HEIGHT / 2) + (int)(height / 2);

	// 边界保护（防止 render_column 像素溢出）
	if (wall_start < 0)
		wall_start = 0;
	if (wall_end >= HEIGHT)
		wall_end = HEIGHT - 1;

	// --- 5. 贴图选择逻辑 ---
	t_tex *tex;
	if (side == 0)
	{
		tex = (ray_dir_x > 0) ? &game->east : &game->west;
	}
	else
	{
		tex = (ray_dir_y > 0) ? &game->south : &game->north;
	}

	// --- 6. 贴图坐标计算 ---
	float wall_x;
	if (side == 0)
		wall_x = player->y + perp_wall_dist * ray_dir_y;
	else
		wall_x = player->x + perp_wall_dist * ray_dir_x;
	wall_x -= floor(wall_x);

	int tex_x = (int)(wall_x * (float)tex->width);

	// 镜像修正
	if ((side == 0 && ray_dir_x < 0) || (side == 1 && ray_dir_y > 0))
		tex_x = tex->width - tex_x - 1;

	// --- 7. 执行渲染 ---
	render_column(i, wall_start, wall_end, tex_x, tex, game);
}

/**
 * MLX 渲染循环：处理清屏、绘图、位置更新
 */
int draw_loop(t_game *game)
{
	t_player *player = &game->player;
	clear_img(game);

	// 1. 计算方向向量 (玩家正前方)
	float dir_x = cos(player->angle);
	float dir_y = sin(player->angle);

	// 2. 计算视平面向量 (垂直于方向向量)
	// 0.66 决定了 FOV。0.66 对应约 66度，你可以根据需要调整。
	// 如果 Y 轴向下，这里的正负号可能需要微调 (例如 -dir_y, dir_x)
	float plane_x = -dir_y * 0.66f;
	float plane_y = dir_x * 0.66f;

	int i = 0;
	while (i < WIDTH)
	{
		// 3. 计算当前列在平面上的位置 (-1 到 1)
		float camera_x = 2.0f * i / (float)WIDTH - 1.0f;

		// 4. 合成射线的方向向量
		float ray_dir_x = dir_x + plane_x * camera_x;
		float ray_dir_y = dir_y + plane_y * camera_x;

		// 5. 计算这一条射线的角度，传给你的 draw_line
		float ray_angle = atan2(ray_dir_y, ray_dir_x);

		draw_line(player, game, ray_angle, i);
		i++;
	}

	if (DEBUG)
	{
		draw_square(player->x * 10, player->y * 10, 5, 0x00FF00, game); // 注意 2D 缩放
		draw_map(game);
	}

	move_player(player, game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return 0;
}
