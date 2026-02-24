/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 20:27:03 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_mini_pixel(t_game *game, int x, int y, int color)
{
	/* 1. 边界检查 (Safety Guard) */
	/* 确保像素点落在预定义的小地图尺寸 (MINI_WIDTH * MINI_HEIGHT) 之内 */
	/* 这样可以防止在绘制小地图内部物体（如玩家、射线）时溢出到地图框外 */
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return ;

	/* 2. 坐标偏移与绘制 (Coordinate Offsetting) */
	/* 调用通用的绘图函数 put_pixel */
	/* 将小地图内部坐标 (x, y) 映射到屏幕上的物理位置 */
	/* 这里使用了硬编码的偏移量 (+20, +20)，意味着小地图显示在屏幕左上角边距 20 像素处 */
	put_pixel(x + 20, y + 20, color, game);
}

static void	draw_mini_player(t_game *game)
{
	t_int_xy	p;    /* 用于绘制方块的循环变量（相对偏移） */
	t_int_xy	c;    /* 小地图的中心点坐标 */
	int			i;    /* 用于绘制视线方向箭头的长度累加器 */

	/* 1. 确定小地图的中心 */
	/* 这里的逻辑通常假设小地图是以玩家为中心进行滚动显示的 */
	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;

	/* 2. 绘制玩家主体：一个 5x5 的红色方块 */
	/* 外层循环处理 Y 轴偏移 (-2 到 2) */
	p.y = -3;
	while (++p.y <= 2)
	{
		/* 内层循环处理 X 轴偏移 (-2 到 2) */
		p.x = -3;
		while (++p.x <= 2)
			/* 0xFF0000 是红色，表示玩家位置 */
			put_mini_pixel(game, c.x + p.x, c.y + p.y, 0xFF0000);
	}

	/* 3. 绘制视线方向：一根长度为 12 像素的绿色线条 */
	i = -1;
	while (++i < 12)
	{
		/* 利用三角函数，根据玩家当前的 angle 计算出视线指向的像素点 */
		/* cos(angle) * i 得到 X 方向的延伸量 */
		/* sin(angle) * i 得到 Y 方向的延伸量 */
		/* 0x00FF00 是绿色，清晰指示玩家面朝哪里 */
		put_mini_pixel(game, c.x + (int)(cos(game->player.angle) * i),
			c.y + (int)(sin(game->player.angle) * i), 0x00FF00);
	}
}

int	get_mini_color(t_game *game, int x, int y)
{
	float	map_x;
	float	map_y;
	char	c;

	/* 1. 坐标转换：将小地图像素坐标转换为世界地图坐标 */
	/* (x - MINI_WIDTH / 2) 计算当前像素相对于小地图中心的偏移量 */
	/* 除以 pix_per_unit 将像素偏移转换为地图格子单位（例如：10像素 = 1格） */
	/* 最后加上玩家的当前坐标 game->player.x，得到该像素在世界地图中的精确浮点位置 */
	map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
	map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;

	/* 2. 边界检查：如果计算出的地图坐标超出了实际地图范围 */
	/* 返回深灰色 (0x1A1A1A)，代表地图外的虚无区域 */
	if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
		return (0x1A1A1A);

	/* 3. 采样：获取该坐标在地图数组中的字符内容 */
	c = game->map[(int)map_y][(int)map_x];

	/* 4. 颜色分配逻辑 */
	/* 如果是墙壁 ('1')，返回白色 (0xFFFFFF) */
	if (c == '1')
		return (0xFFFFFF);
	/* 如果是门 ('D') */
	if (c == '1' || c == 'D') // 逻辑中对 D 的特殊处理
	{
		/* 检查门的状态：如果门是关闭的 (0)，显示浅灰色 (0xAAAAAA) */
		if (game->door_state && game->door_state[(int)map_y][(int)map_x] == 0)
			return (0xAAAAAA);
		/* 如果门开了，显示和空地一样的深蓝色/灰色 (0x333333) */
		return (0x333333);
	}
	/* 其他情况（如空地 '0' 或玩家起始位置），返回地面颜色 (0x333333) */
	return (0x333333);
}

static void	draw_mini_sprites(t_game *game)
{
	int			i;      /* 精灵列表的索引计数器 */
	t_int_xy	s;      /* 精灵在小地图上的中心像素坐标 */
	t_int_xy	d2;     /* 循环变量，用于绘制代表精灵的小方块 */
	t_coords	offset; /* 精灵相对于玩家的世界坐标偏移量 */

	i = 0;
	/* 1. 遍历游戏中所有的精灵 */
	while (i < game->sprs.num)
	{
		/* 2. 计算相对位移：精灵的世界坐标 (x, y) 减去玩家的世界坐标 */
		offset.x = game->sprs.list[i].x - game->player.x;
		offset.y = game->sprs.list[i].y - game->player.y;

		/* 3. 坐标转换：将世界位移转换为小地图上的像素位置 */
		/* 中心点 (MINI_WIDTH / 2) + (偏移量 * 缩放比例) */
		s.x = (int)(MINI_WIDTH / 2 + offset.x * game->pix_per_unit);
		s.y = (int)(MINI_HEIGHT / 2 + offset.y * game->pix_per_unit);

		/* 4. 绘制精灵：在计算出的位置绘制一个 3x3 的绿色方块 */
		/* d2.y 从 -1 到 1，涵盖 3 个像素 */
		d2.y = -2;
		while (++d2.y <= 1)
		{
			d2.x = -2;
			while (++d2.x <= 1)
				/* 使用绿色 (0x00FF00) 表示精灵，以便与红色的玩家区分 */
				put_mini_pixel(game, s.x + d2.x, s.y + d2.y, 0x00FF00);
		}
		i++;
	}
}
void	render_minimap(t_game *game)
{
	t_int_xy	p;

	/* 1. 绘制小地图背景（墙壁与地面） */
	/* 采用双重循环遍历小地图区域内的每一个像素点 */
	p.y = 0;
	while (p.y < MINI_HEIGHT)
	{
		p.x = 0;
		while (p.x < MINI_WIDTH)
		{
			/* 对每一个像素坐标 (p.x, p.y)，调用 get_mini_color */
			/* get_mini_color 会计算该像素对应地图上的哪种材质（墙/地/门）并返回颜色 */
			put_mini_pixel(game, p.x, p.y, get_mini_color(game, p.x, p.y));
			p.x++;
		}
		p.y++;
	}

	/* 2. 叠加动态层（从底向上覆盖） */
	
	/* 绘制玩家图标：通常是地图中心的一个红点/方块 */
	draw_mini_player(game);

	/* 绘制精灵：将敌人、道具等动态物体投影到小地图上 */
	draw_mini_sprites(game);

	/* 绘制中心视线：从玩家中心射出的一根指示正前方方向的直线 */
	draw_mini_vision_line(game);

	/* 绘制视野扇区 (FOV)：显示玩家当前能看到的扇形区域范围 */
	/* 这有助于玩家理解 3D 画面与 2D 地图的对应关系 */
	draw_mini_fov(game);
}
