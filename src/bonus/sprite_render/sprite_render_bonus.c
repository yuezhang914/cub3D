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
