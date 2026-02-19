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
static void draw_sprite_pixels(t_game *game, t_sprite_render_vars v,
                               float trans_y, t_tex *tex)
{
    int     stripe;
    int     y;
    int     tex_x;
    int     tex_y;
    int     color;
    int     animated_y;
    float   wave;

    // wave 控制火焰上下跳动的偏移像素量
    wave = sin(game->time * 12.0f) * 2.0f; 

    stripe = v.draw_start_x;
    while (stripe < v.draw_end_x)
    {
        tex_x = (int)((stripe - v.draw_start_x) * tex->width / v.sprite_w);
        if (stripe >= 0 && stripe < WIDTH && trans_y > 0 && trans_y < game->z_buffer[stripe])
        {
            y = v.draw_start_y;
            while (y < v.draw_end_y)
            {
                // 计算标准的贴图 Y 坐标
                tex_y = (int)((y - v.draw_start_y) * tex->height / v.sprite_h);

                // --- 方案1：基于坐标的局部动效 ---
                // 如果是火炬，并且当前像素属于贴图的上半部分（火焰区域，0-7行）
                if (v.type == SPR_TORCH && tex_y < 8)
                {
                    // 只对火焰部分应用偏移
                    animated_y = tex_y + (int)wave;
                    // 安全检查：防止偏移后超出火焰区域或变为负数
                    if (animated_y < 0) animated_y = 0;
                    if (animated_y > 8) animated_y = 8;
                    color = *(int *)(tex->data + (animated_y * tex->size_line + tex_x * (tex->bpp / 8)));
                }
                else
                {
                    // 手柄部分（tex_y >= 8）或非火炬精灵，保持静止读取
                    color = *(int *)(tex->data + (tex_y * tex->size_line + tex_x * (tex->bpp / 8)));
                }

                // 渲染非透明像素
                if ((color & 0x00FFFFFF) != 0)
                    put_pixel(stripe, y, color, game);
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

    if (conf->is_directional)
        tex = &conf->frames[get_sprite_dir_index(game, s)];
    else
        tex = &conf->frames[s->cur_frame];

    // 保持尺寸固定，不加 wave
    v.sprite_h = abs((int)(HEIGHT / t_y)) / conf->v_div;
    v.sprite_w = abs((int)(HEIGHT / t_y)) / conf->h_div;
    v.v_offset = (int)(conf->v_move / t_y);

    v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    v.draw_end_y = v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    
    if (v.draw_start_y < 0) v.draw_start_y = 0;
    if (v.draw_end_y >= HEIGHT) v.draw_end_y = HEIGHT - 1;

    v.screen_x = (int)((WIDTH / 2) * (1 + t_x / t_y));
    v.draw_start_x = -v.sprite_w / 2 + v.screen_x;
    v.draw_end_x = v.sprite_w / 2 + v.screen_x;

    if (v.draw_start_x < 0) v.draw_start_x = 0;
    if (v.draw_end_x >= WIDTH) v.draw_end_x = WIDTH - 1;

    // 记录当前精灵的类型，方便在像素处理时判断
    v.type = s->type; 
    draw_sprite_pixels(game, v, t_y, tex);
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
