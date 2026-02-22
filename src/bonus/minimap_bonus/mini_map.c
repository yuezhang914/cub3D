/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 22:47:09 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：put_mini_pixel（static）
** 作用：在小地图上画一个像素（做边界检查 + 把小地图坐标映射到屏幕坐标）
**
** 参数：
**   game：包含主画布信息（put_pixel 需要 game->data/bpp/size_line）
**   x, y：小地图内部坐标（范围 0..MINI_WIDTH-1 / 0..MINI_HEIGHT-1）
**   color：要画的颜色（0xRRGGBB）
**
** 主要逻辑：
**   1) 如果 (x,y) 超出小地图范围，直接 return（防止越界）
**   2) put_pixel(x + 20, y + 20, color, game)
**      - 小地图不画在(0,0)，而是整体平移到屏幕左上角偏移 (20,20)
**
** 调用者：
**   render_minimap()：画底图每个像素
**   draw_mini_player()：画玩家方块/方向线
**   draw_mini_sprites()：画 sprites 点
*/
static void	put_mini_pixel(t_game *game, int x, int y, int color)
{
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return ;
	put_pixel(x + 20, y + 20, color, game);
}

/*
** 函数名：draw_mini_player（static）
** 作用：在小地图中心画出玩家：
**   - 一个红色 6×6 方块代表玩家位置
**   - 一条绿色短线代表玩家面朝方向
**
** 参数：
**   game：提供玩家角度 game->player.angle，以及 put_mini_pixel
**
** 主要逻辑：
**   1) c = (MINI_WIDTH/2, MINI_HEIGHT/2)：小地图中心点 = 玩家位置显示点
**   2) 画红色方块：p.x/p.y 从 -3 到 2（总共 6×6）
**   3) 画绿色方向线：
**      - i 从 0..11（线长约 12 像素）
**      - 使用 (cos(angle)*i, sin(angle)*i) 得到方向向量
**
** 抽象概念解释（初中版）：
**   cos(angle) 表示“朝向在 x 方向的比例”，sin(angle) 表示“朝向在 y 方向的比例”
**   乘以 i 就是“往这个方向走 i 个像素”
**
** 调用者：
**   render_minimap()
*/
static void	draw_mini_player(t_game *game)
{
	t_int_xy	p;   /* 用于遍历玩家方块的偏移量 */
	t_int_xy	c;   /* 小地图中心点坐标 */
	int			i;   /* 方向线的长度步数 */

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;

	/* 画玩家红色方块：6×6 */
	p.y = -3;
	while (++p.y <= 2)
	{
		p.x = -3;
		while (++p.x <= 2)
			put_mini_pixel(game, c.x + p.x, c.y + p.y, 0xFF0000);
	}

	/* 画玩家朝向：绿色线 */
	i = -1;
	while (++i < 12)
	{
		put_mini_pixel(game,
			c.x + (int)(cos(game->player.angle) * i),
			c.y + (int)(sin(game->player.angle) * i),
			0x00FF00);
	}
}

/*
** 函数名：get_mini_color（static）
** 作用：决定小地图某个像素 (x,y) 应该画什么颜色
**
** 参数：
**   game：提供玩家位置 player.x/y、地图 map、尺寸 map_w/map_h、门状态 door_state
**   x,y：小地图像素坐标（0..MINI_WIDTH-1 / 0..MINI_HEIGHT-1）
**
** 返回：
**   一个颜色值 0xRRGGBB
**
** 主要逻辑（核心：像素→世界坐标→查地图）：
**   1) 把小地图像素坐标转换为“世界地图坐标”：
**      map_x = player.x + (x - MINI_WIDTH/2) / pix_per_unit
**      map_y = player.y + (y - MINI_HEIGHT/2) / pix_per_unit
**
**      解释：
**      - MINI_WIDTH/2, MINI_HEIGHT/2 是中心，也就是玩家所在位置显示点
**      - x - MINI_WIDTH/2 是像素偏移（右正左负）
**      - 除以 pix_per_unit 把“像素偏移”变成“地图单位偏移”
**
**   2) 如果算出来落在地图外：返回深灰（0x1A1A1A）
**
**   3) 取地图字符：
**      c = map[(int)map_y][(int)map_x]
**      - '1'：墙 → 白色 0xFFFFFF
**      - 'D'：门：
**          * door_state==0（关门）→ 浅灰 0xAAAAAA
**          * door_state==1（开门）→ 深灰 0x333333
**      - 其他（地面等）：深灰 0x333333
**
** 调用者：
**   render_minimap() 在画底图每个像素时调用
*/
static int	get_mini_color(t_game *game, int x, int y)
{
	float	map_x;
	float	map_y;
	char	c;

	/* 小地图像素 -> 世界坐标（地图坐标） */
	map_x = game->player.x
		+ (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
	map_y = game->player.y
		+ (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;

	/* 地图外：画成更暗的背景色 */
	if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
		return (0x1A1A1A);

	c = game->map[(int)map_y][(int)map_x];

	/* 墙 */
	if (c == '1')
		return (0xFFFFFF);

	/* 门：用 door_state 区分开/关颜色 */
	if (c == 'D')
	{
		if (game->door_state && game->door_state[(int)map_y][(int)map_x] == 0)
			return (0xAAAAAA); /* 关门：浅灰更显眼 */
		return (0x333333);     /* 开门：当作地面色 */
	}

	/* 地面/其他：深灰 */
	return (0x333333);
}

/*
** 函数名：draw_mini_sprites（static）
** 作用：在小地图上画出所有 sprite 的位置（BONUS 用）
**
** 参数：
**   game：提供 sprite 列表 game->sprs.list、数量 sprs.num，以及 player.x/y 和 pix_per_unit
**
** 主要逻辑：
**   对每个 sprite：
**   1) offset = sprite_pos - player_pos（相对玩家的偏移，单位：地图格子）
**   2) 把偏移转换为小地图像素坐标：
**      s.x = MINI_WIDTH/2 + offset.x * pix_per_unit
**      s.y = MINI_HEIGHT/2 + offset.y * pix_per_unit
**   3) 在 (s.x, s.y) 周围画一个 4×4 绿色小方块（d2.x/d2.y 从 -2 到 1）
**
** 抽象概念解释：
**   - sprite 与玩家的偏移用“地图单位”（格子）表达
**   - 乘 pix_per_unit 后变成“小地图像素偏移”
**   - 加中心点就得到 sprite 在小地图上的位置
**
** 调用者：
**   render_minimap()
*/
static void	draw_mini_sprites(t_game *game)
{
	int			i;
	t_int_xy	s;      /* sprite 在小地图的中心像素位置 */
	t_int_xy	d2;     /* 用于画小方块的偏移 */
	t_coords	offset; /* sprite 相对玩家的世界坐标偏移 */

	i = 0;
	while (i < game->sprs.num)
	{
		offset.x = game->sprs.list[i].x - game->player.x;
		offset.y = game->sprs.list[i].y - game->player.y;

		/* 世界偏移（格子单位）-> 小地图像素偏移 */
		s.x = (int)(MINI_WIDTH / 2 + offset.x * game->pix_per_unit);
		s.y = (int)(MINI_HEIGHT / 2 + offset.y * game->pix_per_unit);

		/* 画一个绿色 4×4 小方块 */
		d2.y = -2;
		while (++d2.y <= 1)
		{
			d2.x = -2;
			while (++d2.x <= 1)
				put_mini_pixel(game, s.x + d2.x, s.y + d2.y, 0x00FF00);
		}
		i++;
	}
}

/*
** 函数名：render_minimap
** 作用：小地图渲染总入口：先画底图，再画玩家，再画 sprites
**
** 参数：
**   game：提供地图、玩家、门、sprites、画布等
**
** 主要逻辑：
**   1) 双层循环遍历小地图像素 (p.x, p.y)
**      - color = get_mini_color(game, p.x, p.y)
**      - put_mini_pixel 画到屏幕上（带 +20 偏移）
**   2) draw_mini_player：在中心画玩家和朝向
**   3) draw_mini_sprites：画 sprites（如果有）
**
** 调用者：
**   loop.c 的 handle_bonus_rendering()（BONUS）中调用 render_minimap
*/
void	render_minimap(t_game *game)
{
	t_int_xy	p;

	p.y = 0;
	while (p.y < MINI_HEIGHT)
	{
		p.x = 0;
		while (p.x < MINI_WIDTH)
		{
			put_mini_pixel(game, p.x, p.y, get_mini_color(game, p.x, p.y));
			p.x++;
		}
		p.y++;
	}
	draw_mini_player(game);
	draw_mini_sprites(game);
}
