/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_render_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:11:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 00:36:03 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：get_sprite_color（static）
** 作用：从 sprite 当前贴图中取一个像素颜色（tx,ty），并对火把做“上下跳动”特效
**
** 参数：
**   ctx：绘制上下文，包含 game、当前 sprite 的投影参数 v、当前贴图 tex
**   tx：贴图 x 坐标
**   ty：贴图 y 坐标
**
** 返回：
**   像素颜色（int），越界返回 0（当成透明）
**
** 主要逻辑：
**   1) 默认 y=ty
**   2) 如果类型是 SPR_TORCH：
**      - jump = sin(time*15)*4*(1 - ty/tex_h)
**        * sin(time*15) 让它随时间左右摆动（周期性）
**        * *4 控制幅度
**        * (1 - ty/tex_h) 让上半部分抖动更大、下半部分抖动更小
**          （更像火焰：底部稳定，顶部摇晃）
**      - y = ty + jump，并 clamp 到 [0, tex_h-1]
**   3) 越界检查：tx 或 y 超界 → 返回 0
**   4) 从 tex->data 取像素：
**      data + y*size_line + tx*(bpp/8)
**
** 调用者：
**   draw_sprite_pixels()
*/
static int	get_sprite_color(t_draw_ctx *ctx, int tx, int ty)
{
	int	y;
	int	jump;

	y = ty;
	if (ctx->v.type == SPR_TORCH)
	{
		jump = (int)(sin(ctx->game->time * 15.0f) * 4.0f
				* (1.0f - ((float)ty / ctx->tex->height)));
		y = ty + jump;
		if (y < 0)
			y = 0;
		if (y >= ctx->tex->height)
			y = ctx->tex->height - 1;
	}
	if (tx < 0 || tx >= ctx->tex->width || y < 0 || y >= ctx->tex->height)
		return (0);
	return (*(int *)(ctx->tex->data + (y * ctx->tex->size_line
			+ tx * (ctx->tex->bpp / 8))));
}

/*
** 函数名：draw_sprite_pixels（static）
** 作用：把一个 sprite 在屏幕上的矩形区域逐像素画出来（包含 z_buffer 遮挡与透明判断）
**
** 参数：
**   ctx：包含：
**     - ctx->v：sprite 投影参数（draw_start_x/y、sprite_w/h、type 等）
**     - ctx->tex：当前用的贴图
**     - ctx->trans_y：sprite 深度（越大越远）
**     - sx/ex/sy/ey：屏幕裁剪后的绘制范围
**     - game：用来 put_pixel 和读 z_buffer
**
** 主要逻辑：
**   对每个屏幕 x（sx..ex）：
**     1) 计算贴图 tx：
**        tx = (x - draw_start_x) * tex_w / sprite_w
**     2) 遮挡判断（非常关键）：
**        - trans_y <= 0：在背后，不画
**        - trans_y >= z_buffer[x]：在墙后，被挡住，不画
**     3) 对每个屏幕 y（sy..ey）：
**        ty = (y - draw_start_y) * tex_h / sprite_h
**        col = get_sprite_color(ctx, tx, ty)
**        如果 (col & 0x00FFFFFF) != 0 才画
**        - 这相当于把“纯黑/透明背景”当成透明
**
** 调用者：
**   draw_single_sprite()
*/
static void	draw_sprite_pixels(t_draw_ctx *ctx)
{
	int	x;
	int	y;
	int	tx;
	int	ty;
	int	col;

	x = ctx->sx - 1;
	while (++x < ctx->ex)
	{
		tx = (int)((x - ctx->v.draw_start_x) * ctx->tex->width
				/ ctx->v.sprite_w);
		if (x < 0 || x >= WIDTH || ctx->trans_y <= 0
			|| ctx->trans_y >= ctx->game->z_buffer[x])
			continue ;
		y = ctx->sy - 1;
		while (++y < ctx->ey)
		{
			ty = (int)((y - ctx->v.draw_start_y) * ctx->tex->height
					/ ctx->v.sprite_h);
			col = get_sprite_color(ctx, tx, ty);
			if ((col & 0x00FFFFFF) != 0)
				put_pixel(x, y, col, ctx->game);
		}
	}
}

/*
** 函数名：get_sprite_dir_index
** 作用：对“8方向怪物贴图”，计算当前应该用哪一张方向图（0..7）
**
** 参数：
**   game：提供玩家位置（用来计算玩家看向 sprite 的角度）
**   s：sprite（用 s->angle 作为 sprite 自己的朝向基准）
**
** 返回：
**   index：0..7，对应 frames[index]
**
** 主要逻辑（角度数学）：
**   1) view_angle = atan2(s->y - player.y, s->x - player.x)
**      - 这是“从玩家指向 sprite 的方向角”
**   2) relative_angle = view_angle - s->angle + PI
**      - 做成相对角：比较“玩家看它的方向”和“怪物自身朝向”
**      - +PI 是为了把范围移到正区间附近（实现习惯）
**   3) relative_angle = fmod(relative_angle, 2PI) 并确保 >=0
**   4) 划分成 8 份，每份 45°（PI/4）：
**      index = ((relative_angle + PI/8) / (PI/4)) % 8
**      - +PI/8 是四舍五入到最近方向，而不是直接向下取整
**
** 调用者：
**   draw_single_sprite()：当 sprite 是 directional 时选择贴图帧
*/
int	get_sprite_dir_index(t_game *game, t_sprite *s)
{
	float	view_angle;
	float	relative_angle;
	int		index;

	view_angle = atan2(s->y - game->player.y, s->x - game->player.x);
	relative_angle = view_angle - s->angle + PI;
	relative_angle = fmodf(relative_angle, 2.0f * PI);
	if (relative_angle < 0)
		relative_angle += 2.0f * PI;
	index = (int)((relative_angle + PI / 8.0f) / (PI / 4.0f)) % 8;
	return (index);
}

/*
** 函数名：draw_single_sprite（static）
** 作用：把一个 sprite 投影到屏幕，并组织绘制上下文 ctx，然后画像素
**
** 参数：
**   game：提供 config、玩家、z_buffer、put_pixel 等
**   s：要画的 sprite
**   t_x, t_y：sprite 在相机空间的坐标（由 render_sprites 计算得到）
**             - t_y 是深度（越大越远），用于缩放与遮挡
**
** 主要逻辑（投影 & 尺寸）：
**   1) c = &game->config[s->type] 找到该类型配置
**   2) 选择贴图：
**      - 若 directional：tex = frames[get_sprite_dir_index(...)]
**      - 否则：tex = frames[s->cur_frame]
**   3) 计算屏幕上的 sprite 高宽（与距离成反比）：
**      sprite_h = abs(HEIGHT / t_y) / v_div
**      sprite_w = abs(HEIGHT / t_y) / h_div
**   4) 垂直偏移：
**      v_offset = v_move / t_y（越远偏移越小）
**   5) 计算屏幕左上角：
**      draw_start_y = -sprite_h/2 + HEIGHT/2 + v_offset
**      draw_start_x = -sprite_w/2 + (WIDTH/2) * (1 + t_x/t_y)
**      - (WIDTH/2) * (1 + t_x/t_y) 是把相机 x 映射到屏幕 x
**   6) 计算裁剪后的绘制范围 sx/ex/sy/ey（用 fmax/fmin 卡到屏幕内）
**   7) draw_sprite_pixels(&ctx)
**
** 调用者：
**   render_sprites()
*/
static void	draw_single_sprite(t_game *game, t_sprite *s, float t_x, float t_y)
{
	t_sprite_render_vars	v;
	t_sprite_config			*c;
	t_draw_ctx				ctx;

	c = &game->config[s->type];
	if (c->is_directional)
		ctx.tex = &c->frames[get_sprite_dir_index(game, s)];
	else
		ctx.tex = &c->frames[s->cur_frame];

	v.sprite_h = abs((int)(HEIGHT / t_y)) / c->v_div;
	v.sprite_w = abs((int)(HEIGHT / t_y)) / c->h_div;
	v.v_offset = (int)(c->v_move / t_y);
	v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
	v.draw_start_x = -v.sprite_w / 2 + (int)((WIDTH / 2) * (1 + t_x / t_y));
	v.type = s->type;

	ctx.game = game;
	ctx.v = v;
	ctx.trans_y = t_y;
	ctx.sx = fmax(0, v.draw_start_x);
	ctx.ex = fmin(WIDTH - 1, v.sprite_w / 2
			+ (v.draw_start_x + v.sprite_w / 2));
	ctx.sy = fmax(0, v.draw_start_y);
	ctx.ey = fmin(HEIGHT - 1, v.sprite_h / 2 + HEIGHT / 2 + v.v_offset);

	draw_sprite_pixels(&ctx);
}

#ifdef BONUS
/*
** 函数名：render_sprites（BONUS）
** 作用：每帧渲染所有 sprite：先排序，再把每个 sprite 转换到相机空间并绘制
**
** 参数：
**   game：提供 player、sprs.list、z_buffer 等
**
** 主要逻辑（世界→相机 2×2 变换）：
**   1) update_sprite_distances + sort_sprites：远到近
**   2) 计算 inv（2×2 矩阵的逆的系数，等价于 1/determinant）：
**      这里的 0.66f 是相机平面长度（≈ tan(FOV/2) 的某个常用值）
**      你在别处 plane 的长度也常用 0.66（约 66° FOV）
**
**   3) 对每个 sprite：
**      dx = s->x - player.x
**      dy = s->y - player.y
**      tx = inv * ( sin(angle)*dx - cos(angle)*dy )
**      ty = inv * ( -(cos(angle)*0.66)*dx + (-sin(angle)*0.66)*dy )
**
**      你可以把 ty 理解为“深度”（前方距离），tx 是“左右偏移”
**
**   4) 只有 ty > 0.1 才画（太近/在背后都不画，避免除 0 与异常）
**   5) draw_single_sprite(game, s, tx, ty)
**
** 调用者：
**   loop.c 的 bonus 渲染阶段（handle_bonus_rendering 里）
*/
void	render_sprites(t_game *game)
{
	int			i;
	t_sprite	*s;
	float		inv;
	float		tx;
	float		ty;

	update_sprite_distances(game);
	sort_sprites(game);

	inv = 1.0f / ((-sin(game->player.angle) * 0.66f) * sin(game->player.angle)
			- cos(game->player.angle) * (cos(game->player.angle) * 0.66f));

	i = -1;
	while (++i < game->sprs.num)
	{
		s = &game->sprs.list[i];
		tx = inv * (sin(game->player.angle) * (s->x - game->player.x)
				- cos(game->player.angle) * (s->y - game->player.y));
		ty = inv * (-(cos(game->player.angle) * 0.66f) * (s->x - game->player.x)
				+ (-sin(game->player.angle) * 0.66f) * (s->y - game->player.y));
		if (ty > 0.1f)
			draw_single_sprite(game, s, tx, ty);
	}
}
#endif