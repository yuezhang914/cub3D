/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_render_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:11:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 10:12:30 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * 绘制精灵像素并应用距离阴影
 * @param v 渲染所需的各种坐标变量
 * @param trans_y 精灵在摄像机空间中的深度（用于 Z-Buffer 检查和阴影计算）
 * @param tex 当前选中的 XPM 贴图指针
 */
/* 修改后的函数头，接收裁剪后的 act_ 边界参数 */
static void draw_sprite_pixels(t_draw_ctx *ctx)
{
    int stripe, y;
    int tex_x, tex_y;
    int color;
    float jump_factor;

    // 1. 计算当前帧的总跳动偏移量
    // sin(game->time * 15.0f) 控制速度，* 4.0f 控制最大跳动像素距离
    float total_jump = sin(ctx->game->time * 15.0f) * 4.0f;

    stripe = ctx->sx;
    while (stripe < ctx->ex)
    {
        tex_x = (int)((stripe - ctx->v.draw_start_x) * ctx->tex->width / ctx->v.sprite_w);

        if (stripe >= 0 && stripe < WIDTH && ctx->trans_y > 0 && ctx->trans_y < ctx->game->z_buffer[stripe])
        {
            y = ctx->sy;
            while (y < ctx->ey)
            {
                // 原始纹理坐标计算
                tex_y = (int)((y - ctx->v.draw_start_y) * ctx->tex->height / ctx->v.sprite_h);

                // --- 火焰跳动逻辑 ---
                if (ctx->v.type == SPR_TORCH)
                {
                    /* jump_factor: 顶部 (tex_y=0) 为 1.0, 底部 (tex_y=31) 为 0.0
                    ** 这样火苗会跳动，而手柄（底部）会被锁定不动 */
                    jump_factor = 1.0f - ((float)tex_y / ctx->tex->height);
                    
                    // 仅在 Y 方向应用偏移
                    int animated_y = tex_y + (int)(total_jump * jump_factor);
                    
                    // 边界安全检查
                    if (animated_y < 0) animated_y = 0;
                    if (animated_y >= ctx->tex->height) animated_y = ctx->tex->height - 1;
                    
                    color = *(int *)(ctx->tex->data + (animated_y * ctx->tex->size_line + tex_x * (ctx->tex->bpp / 8)));
                }
                else
                {
                    /* 非火炬精灵正常渲染 */
                    if (tex_x >= 0 && tex_x < ctx->tex->width && tex_y >= 0 && tex_y < ctx->tex->height)
                        color = *(int *)(ctx->tex->data + (tex_y * ctx->tex->size_line + tex_x * (ctx->tex->bpp / 8)));
                    else
                        color = 0;
                }

                if ((color & 0x00FFFFFF) != 0)
                    put_pixel(stripe, y, color, ctx->game);
                y++;
            }
        }
        stripe++;
    }
}

int get_sprite_dir_index(t_game *game, t_sprite *s)
{
    float   view_angle;
    float   relative_angle;
    int     index;

    // 1. 玩家看怪物的角度
    view_angle = atan2(s->y - game->player.y, s->x - game->player.x);

    // 2. 相对角度：(玩家看怪物的角度) - (怪物的朝向)
    // 加上 PI 是为了让“玩家正对着怪物脸”时，索引落在 0 (Front)
    relative_angle = view_angle - s->angle + PI;

    // 3. 标准化 [0, 2*PI]
    relative_angle = fmodf(relative_angle, 2.0f * PI);
    if (relative_angle < 0) relative_angle += 2.0f * PI;

    // 4. 转换为 0-7 索引
    index = (int)((relative_angle + PI / 8.0f) / (PI / 4.0f)) % 8;

    return (index);
}

static void draw_single_sprite(t_game *game, t_sprite *s, float t_x, float t_y)
{
    t_sprite_render_vars v;
    t_sprite_config      *conf = &game->config[s->type];
    t_tex                *tex;
	t_draw_ctx ctx;

    // 1. 根据方向或帧获取纹理
    if (conf->is_directional)
        tex = &conf->frames[get_sprite_dir_index(game, s)];
    else
        tex = &conf->frames[s->cur_frame];

    // 2. 计算精灵在屏幕上的基础尺寸
    v.sprite_h = abs((int)(HEIGHT / t_y)) / conf->v_div;
    v.sprite_w = abs((int)(HEIGHT / t_y)) / conf->h_div;
    
    // 3. 计算垂直偏移 (v_move 此时应为正数以压低位置)
    v.v_offset = (int)(conf->v_move / t_y);

    // 4. 计算原始的绘制边界 (可能是负数或超过屏幕宽高度)
    // 这里的 draw_start/end 必须保持原始值，用于后续 tex_x/y 的百分比映射
    v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    v.draw_end_y = v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;

    v.screen_x = (int)((WIDTH / 2) * (1 + t_x / t_y));
    v.draw_start_x = -v.sprite_w / 2 + v.screen_x;
    v.draw_end_x = v.sprite_w / 2 + v.screen_x;

    // 5. 创建用于循环的“安全边界”变量 (act = actual)
    // 这样我们就不会修改 v 里的原始数据
    int act_start_x = v.draw_start_x;
    int act_end_x = v.draw_end_x;
    int act_start_y = v.draw_start_y;
    int act_end_y = v.draw_end_y;

    // 裁剪到屏幕范围内
    if (act_start_x < 0) act_start_x = 0;
    if (act_end_x >= WIDTH) act_end_x = WIDTH - 1;
    if (act_start_y < 0) act_start_y = 0;
    if (act_end_y >= HEIGHT) act_end_y = HEIGHT - 1;

    // 6. 保存类型并调用像素绘制函数
    v.type = s->type; 

    /* ** 注意：你需要修改 draw_sprite_pixels 的参数，
    ** 传入 act_start_x 等裁剪后的边界，
    ** 但在函数内部计算 tex_x 时，依然减去 v.draw_start_x。
    */
   ctx.game = game;
   ctx.v = v;
   ctx.trans_y = t_y;
   ctx.tex = tex;
   ctx.sx = act_start_x;
   ctx.ex = act_end_x;
   ctx.sy= act_start_y;
   ctx.ey = act_end_y;
    draw_sprite_pixels(&ctx);
}

void render_sprites(t_game *game)
{
    int i;
    t_sprite *s;
    t_coords dir;
    t_coords plane;
    float inv_det;
    float trans_x;
    float trans_y;

    // 1. 更新所有精灵到玩家的距离并排序 (重要：Bonus 必须有排序)
    update_sprite_distances(game);
    sort_sprites(game);
    dir.x = cos(game->player.angle);
    dir.y = sin(game->player.angle);
    plane.x = -dir.y * 0.66f;
    plane.y = dir.x * 0.66f;
    inv_det = 1.0f / (plane.x * dir.y - dir.x * plane.y);

    i = -1;
    while (++i < game->sprs.num)
    {
        s = &game->sprs.list[i];
        float dx = s->x - game->player.x;
        float dy = s->y - game->player.y;

        trans_x = inv_det * (dir.y * dx - dir.x * dy);
        trans_y = inv_det * (-plane.y * dx + plane.x * dy);

        if (trans_y <= 0.1f)
            continue;  
        // 传入 s 以便 draw_single_sprite 知道该画哪种精灵
        draw_single_sprite(game, s, trans_x, trans_y);
		
    }
}
