/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_render_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:11:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:12:30 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_sprite_color(t_draw_ctx *ctx, int tx, int ty)
{
	int	y;
	int	jump;

	y = ty;
	/* 1. 特殊效果逻辑：火炬的动态晃动 (Wobble Effect) */
	/* 只有类型为 SPR_TORCH 时，才会对 Y 轴采样坐标进行动态偏移 */
	if (ctx->v.type == SPR_TORCH)
	{
		/* 利用正弦函数 sin 随时间 (game->time) 产生波动 */
		/* (1.0f - ty/height) 是一个权重因子： */
		/* 这意味着火炬的顶部 (ty 趋近 0) 晃动幅度最大，底部 (ty 趋近 height) 几乎不动 */
		jump = (int)(sin(ctx->game->time * 15.0f) * 4.0f
				* (1.0f - ((float)ty / ctx->tex->height)));
		y = ty + jump;
		
		/* 边界保护：确保扰动后的坐标不会超出纹理内存范围 */
		if (y < 0)
			y = 0;
		if (y >= ctx->tex->height)
			y = ctx->tex->height - 1;
	}

	/* 2. 基础合法性检查 */
	/* 如果纹理坐标越界，返回 0 (通常代表透明色或黑色) */
	if (tx < 0 || tx >= ctx->tex->width || y < 0 || y >= ctx->tex->height)
		return (0);

	/* 3. 像素提取 */
	/* 根据计算出的最终 y 和原始 tx，从纹理数据区提取 4 字节的颜色值 (ARGB) */
	return (*(int *)(ctx->tex->data + (y * ctx->tex->size_line
			+ tx * (ctx->tex->bpp / 8))));
}

static void	draw_sprite_pixels(t_draw_ctx *ctx)
{
	int	x;   /* 屏幕横坐标 */
	int	y;   /* 屏幕纵坐标 */
	int	tx;  /* 纹理横坐标 (Texture X) */
	int	ty;  /* 纹理纵坐标 (Texture Y) */
	int	col; /* 采样到的颜色值 */

	x = ctx->sx - 1;
	/* 1. 横向扫描：遍历精灵在屏幕上占据的所有列 */
	while (++x < ctx->ex)
	{
		/* 计算当前像素列对应的纹理 X 坐标 */
		/* 原理：(当前屏幕x - 精灵起始x) / 精灵显示总宽度 * 纹理原始宽度 */
		tx = (int)((x - ctx->v.draw_start_x) * ctx->tex->width
				/ ctx->v.sprite_w);

		/* 2. 深度测试与可见性检查 (Critical!) */
		/* x < 0 || x >= WIDTH: 确保不画出屏幕左右边界 */
		/* ctx->trans_y <= 0: 确保精灵在玩家前方（防止反向投影） */
		/* ctx->trans_y >= ctx->game->z_buffer[x]: 如果精灵深度大于该列墙壁深度，说明被墙遮挡 */
		if (x < 0 || x >= WIDTH || ctx->trans_y <= 0
			|| ctx->trans_y >= ctx->game->z_buffer[x])
			continue ;

		y = ctx->sy - 1;
		/* 3. 纵向扫描：遍历当前列中精灵占据的所有行 */
		while (++y < ctx->ey)
		{
			/* 计算当前像素行对应的纹理 Y 坐标 */
			ty = (int)((y - ctx->v.draw_start_y) * ctx->tex->height
					/ ctx->v.sprite_h);

			/* 4. 采样与透明度处理 */
			/* 调用 get_sprite_color 获取像素（可能包含火炬晃动等特效） */
			col = get_sprite_color(ctx, tx, ty);

			/* 颜色过滤：0x00FFFFFF 以外的部分为透明色（通常 XPM 背景为黑色/纯色） */
			/* 只有非透明像素才会被写入缓冲区 */
			if ((col & 0x00FFFFFF) != 0)
				put_pixel(x, y, col, ctx->game);
		}
	}
}

int	get_sprite_dir_index(t_game *game, t_sprite *s)
{
	float	view_angle;      /* 玩家相对于精灵的观察角度 */
	float	relative_angle;  /* 修正后的相对旋转夹角 */
	int		index;           /* 最终映射到 0-7 的纹理索引 */

	/* 1. 计算观察角度 (View Angle) */
	/* 使用 atan2 计算从精灵指向玩家的向量角度 */
	/* 结果范围是 (-PI, PI] */
	view_angle = atan2(s->y - game->player.y, s->x - game->player.x);

	/* 2. 计算相对角度 (Relative Angle) */
	/* 公式：观察者的角度 - 精灵自身的朝向角度 + PI (用于对齐 0 度为正面) */
	relative_angle = view_angle - s->angle + PI;

	/* 3. 角度归一化 */
	/* 使用 fmodf 确保角度落在 [0, 2*PI] 之间，处理跨越 0 度线的情况 */
	relative_angle = fmodf(relative_angle, 2.0f * PI);
	if (relative_angle < 0)
		relative_angle += 2.0f * PI;

	/* 4. 将弧度转换为 8 个象限的索引 */
	/* (PI / 4.0f) 是 45 度，即将圆平分为 8 份 */
	/* 加上 (PI / 8.0f) 是为了做一个 22.5 度的偏移量（四舍五入效果）， */
	/* 使得玩家在正负 22.5 度范围内看到的都是同一个面。 */
	index = (int)((relative_angle + PI / 8.0f) / (PI / 4.0f)) % 8;

	return (index);
}
static void	draw_single_sprite(t_game *game, t_sprite *s, float t_x, float t_y)
{
	t_sprite_render_vars	v;   /* 存储计算出的屏幕绘制参数 */
	t_sprite_config			*c;   /* 对应精灵类型的配置（缩放、偏移等） */
	t_draw_ctx				ctx; /* 传递给像素绘制函数的上下文 */

	ft_bzero(&ctx, sizeof(t_draw_ctx));
	/* 1. 获取精灵配置并选择正确的纹理帧 */
	c = &game->config[s->type];
	if (c->is_directional)
		/* 如果是多方向精灵（如怪物），计算玩家与其夹角来选帧 */
		ctx.tex = &c->frames[get_sprite_dir_index(game, s)];
	else
		/* 如果是普通动画精灵（如火炬），直接使用当前动画帧 */
		ctx.tex = &c->frames[s->cur_frame];

	/* 2. 计算精灵在屏幕上的尺寸 */
	/* 这里的 t_y 是变换后的深度（Z坐标），距离越远，t_y 越大，尺寸越小 */
	/* v_div 和 h_div 用于调整精灵大小（比如让木桶看起来比墙矮） */
	v.sprite_h = abs((int)(HEIGHT / t_y)) / c->v_div;
	v.sprite_w = abs((int)(HEIGHT / t_y)) / c->h_div;

	/* 3. 计算垂直位置与偏移 */
	/* v_move 是配置里的垂直偏移，除以 t_y 确保偏移量随距离缩放 */
	v.v_offset = (int)(c->v_move / t_y);
	v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;

	/* 4. 计算水平位置 (关键投影公式) */
	/* t_x / t_y 决定了精灵偏离屏幕中心的比例 */
	v.draw_start_x = -v.sprite_w / 2 + (int)((WIDTH / 2) * (1 + t_x / t_y));

	/* 5. 设置绘制上下文 */
	v.type = s->type;
	ctx.game = game;
	ctx.v = v;
	ctx.trans_y = t_y;

	/* 6. 计算裁剪边界 (Bounding Box) */
	/* 确保起始和结束坐标不会超出屏幕像素范围 [0, WIDTH/HEIGHT] */
	ctx.sx = fmax(0, v.draw_start_x);
	ctx.ex = fmin(WIDTH - 1, v.sprite_w + v.draw_start_x);
	ctx.sy = fmax(0, v.draw_start_y);
	ctx.ey = fmin(HEIGHT - 1, v.sprite_h + v.draw_start_y);

	/* 7. 进入像素填充阶段 */
	draw_sprite_pixels(&ctx);
}

#ifdef BONUS

void	render_sprites(t_game *game)
{
	int			i;
	t_sprite	*s;
	float		inv;  /* 逆矩阵行列式，用于坐标变换 */
	float		tx;   /* 变换后的相机空间 X 坐标（决定左右位置） */
	float		ty;   /* 变换后的相机空间 Y 坐标（即深度，决定远近） */

	/* 1. 预处理：更新所有精灵与玩家的距离并从远到近排序（油画算法） */
	update_sprite_distances(game);
	sort_sprites(game);

	/* 2. 计算相机逆矩阵的行列式系数 */
	/* 这个复杂的公式涉及玩家的方向向量和相机平面向量 (0.66 是 FOV 系数) */
	/* 它是为了将世界坐标系下的位移向量旋转并平移到以玩家为原点的坐标系中 */
	inv = 1.0f / ((-sin(game->player.angle) * 0.66f) * sin(game->player.angle)
			- cos(game->player.angle) * (cos(game->player.angle) * 0.66f));

	i = -1;
	/* 3. 遍历排序后的精灵列表 */
	while (++i < game->sprs.num)
	{
		s = &game->sprs.list[i];

		/* 4. 执行坐标变换 (Matrix-Vector Multiplication) */
		/* 将精灵相对于玩家的相对坐标 (dx, dy) 投影到相机的 Right 和 Forward 轴上 */
		/* tx: 对应屏幕上的水平偏移（负数为左，正数为右） */
		tx = inv * (sin(game->player.angle) * (s->x - game->player.x)
				- cos(game->player.angle) * (s->y - game->player.y));
		
		/* ty: 对应玩家前方的深度距离（必须为正数才在视野内） */
		ty = inv * (-(cos(game->player.angle) * 0.66f) * (s->x - game->player.x)
				+ (-sin(game->player.angle) * 0.66f) * (s->y - game->player.y));

		/* 5. 裁剪与绘制 */
		/* 如果 ty > 0.1，说明精灵在玩家前方且不是贴着脸（防止除以 0 或溢出） */
		if (ty > 0.1f)
			draw_single_sprite(game, s, tx, ty);
	}
}
#endif