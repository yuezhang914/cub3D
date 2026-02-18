#include "cub3d.h"

/**
 * @brief 将新创建的 MLX 图像指针加入追踪链表
 * @param game    游戏主结构体
 * @param img_ptr 由 mlx_xpm_file_to_image 或 mlx_new_image 返回的指针
 */
void add_to_img_list(t_game *game, void *img_ptr)
{
	t_img *new_node;

	if (!img_ptr)
		return;
	/* 1. 分配链表节点内存 */
	/* 建议使用 track_malloc，这样节点本身的内存也会被 track_clean 回收 */
	new_node = (t_img *)track_malloc(game, sizeof(t_img));
	if (!new_node)
	{
		mlx_destroy_image(game->mlx, img_ptr); // 如果分配节点失败，先销毁图片防止泄漏
		graceful_exit(game, 1, __func__, "Failed to allocate image node.");
	}

	/* 2. 初始化节点 */
	new_node->ptr = img_ptr;

	/* 3. 使用头插法加入链表 */
	new_node->next = game->img_head;
	game->img_head = new_node;
}

void init_sprite_texture(t_game *game)
{
	t_tex *t = &game->sprs.tex;
	printf("Loading sprite texture...\n");
	game->mlx = mlx_init();
	if (!game->mlx)
		graceful_exit(game, 1, __func__, "Failed to initialize MLX");

	t->img_ptr = mlx_xpm_file_to_image(game->mlx, "textures/tree.xpm",
									   &t->width, &t->height);
	if (!t->img_ptr)
		graceful_exit(game, 1, __func__, "Failed to load sprite.");

	// 获取数据地址
	t->data = mlx_get_data_addr(t->img_ptr, &t->bpp, &t->size_line, &t->endian);

	printf("tex.data=%p width=%d height=%d bpp=%d size_line=%d\n",
		   t->data, t->width, t->height, t->bpp, t->size_line);
	// ✅ 关键：将精灵图指针加入链表，这样 destroy_images 就能自动清理它
	add_to_img_list(game, t->img_ptr);
}

void collect_sprites(t_game *game)
{
	int i;
	int j;
	int count;

	count = 0;
	// 1. 第一次遍历：数数
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
			if (game->map[i][j] == 'C' || game->map[i][j] == 'D')
				count++;
	}
	game->sprs.num = count;
	game->sprs.list = (t_sprite *)track_malloc(game, sizeof(t_sprite) * count);

	// 2. 第二次遍历：记录坐标并从地图抹除
	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (game->map[i][j] == 'C' || game->map[i][j] == 'D')
			{
				game->sprs.list[count].x = j + 0.5f;
				game->sprs.list[count].y = i + 0.5f;
				game->map[i][j] = '0'; // 变回空地，让射线穿过
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

	for (int i = 0; i < game->sprs.num; i++)
		printf("sprite %d: map_x=%f map_y=%f\n",
			   i, game->sprs.list[i].x, game->sprs.list[i].y);

	while (i < game->sprs.num)
	{
		// 距离平方公式: (x1 - x2)^2 + (y1 - y2)^2
		game->sprs.list[i].dist = ((px - game->sprs.list[i].x) *
									   (px - game->sprs.list[i].x) +
								   (py - game->sprs.list[i].y) *
									   (py - game->sprs.list[i].y));
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

static void draw_sprite_pixels(t_game *game, t_sprite_render_vars v, float trans_y)
{
	int stripe;
	int y;
	int tex_x;
	int tex_y;
	int color;
	int d;

	stripe = v.draw_start_x;
	while (stripe < v.draw_end_x)
	{
		// 1. 计算当前列对应的纹理 X
		tex_x = (int)(256 * (stripe - (-v.sprite_w / 2 + v.screen_x)) * game->sprs.tex.width / v.sprite_w) / 256;

		// 2. 检查：在屏幕内 && 在玩家前方 && 没被墙遮挡
		if (trans_y > 0 && stripe >= 0 && stripe < WIDTH && trans_y < game->z_buffer[stripe])
		{
			y = v.draw_start_y;
			while (y < v.draw_end_y)
			{
				// 3. 计算纹理 Y (通过当前 y 距离屏幕中心的偏差计算)
				d = (y) * 256 - HEIGHT * 128 + v.sprite_h * 128;
				tex_y = ((d * game->sprs.tex.height) / v.sprite_h) / 256;

				// 4. 从数据中提取颜色 (注意：这里直接使用你结构体里的 sprs.tex)
				color = *(int *)(game->sprs.tex.data + (tex_y * game->sprs.tex.size_line + tex_x * (game->sprs.tex.bpp / 8)));

				// 5. 透明色过滤 (0x0 为 XPM 默认透明背景)
				if ((color & 0x00FFFFFF) != 0)
					put_pixel(stripe, y, color, game);
				y++;
			}
		}
		stripe++;
	}
}

static void draw_single_sprite(t_game *game, float t_x, float t_y)
{
	t_sprite_render_vars v;

	// 1. 计算高度和垂直范围
	v.sprite_h = abs((int)(HEIGHT / t_y));
	v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2;
	if (v.draw_start_y < 0)
		v.draw_start_y = 0;
	v.draw_end_y = v.sprite_h / 2 + HEIGHT / 2;
	if (v.draw_end_y >= HEIGHT)
		v.draw_end_y = HEIGHT - 1;

	// 2. 计算宽度和水平范围 (保持 1:1 比例)
	v.sprite_w = abs((int)(HEIGHT / t_y));
	v.screen_x = (int)((WIDTH / 2) * (1 + t_x / t_y));

	v.draw_start_x = -v.sprite_w / 2 + v.screen_x;
	if (v.draw_start_x < 0)
		v.draw_start_x = 0;
	v.draw_end_x = v.sprite_w / 2 + v.screen_x;
	if (v.draw_end_x >= WIDTH)
		v.draw_end_x = WIDTH;

	// 3. 执行绘制
	draw_sprite_pixels(game, v, t_y);
}

void render_sprites(t_game *game)
{
	int i;
	t_sprite *s;
	t_coords dir;	/* 局部变量：方向向量 */
	t_coords plane; /* 局部变量：相机平面向量 */
	float inv_det;
	float trans_x;
	float trans_y;

	printf("Rendering %d sprites\n", game->sprs.num);

	// 1. 根据玩家当前的 angle 计算方向向量 (dir)
	// 注意：这里的 angle 应该是弧度制。如果是角度，请用 (angle * PI / 180)
	dir.x = cos(game->player.angle);
	dir.y = sin(game->player.angle);

	// 2. 计算垂直于方向的相机平面 (plane)
	// 0.66 是根据 FOV 66度算出的比例系数
	plane.x = -dir.y * 0.66f;
	plane.y = dir.x * 0.66f;

	// 3. 计算逆矩阵行列式
	inv_det = 1.0f / (plane.x * dir.y - dir.x * plane.y);

	i = -1;
	while (++i < game->sprs.num)
	{
		s = &game->sprs.list[i];

		// 计算精灵相对于玩家的位移
		float dx = s->x - game->player.x;
		float dy = s->y - game->player.y;

		// 4. 变换到当前视角空间（现在这里可以使用局部变量 dir 和 plane 了）
		trans_x = inv_det * (dir.y * dx - dir.x * dy);
		trans_y = inv_det * (-plane.y * dx + plane.x * dy);

		// 如果精灵在玩家身后，跳过不画
		if (trans_y <= 0.1f)
			continue;

		draw_single_sprite(game, trans_x, trans_y);
	}
}