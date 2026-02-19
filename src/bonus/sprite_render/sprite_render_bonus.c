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
    float   shading;

    // 1. 计算亮度强度 (基于距离的平方反比)
    // 0.05 是衰减系数，你可以根据迷宫的黑暗程度微调
    shading = 1.0f / (1.0f + trans_y * trans_y * 0.05f);
    if (shading > 1.0f) shading = 1.0f;
    if (shading < 0.15f) shading = 0.15f; // 哪怕再远，也保留一点可见轮廓

    stripe = v.draw_start_x;
    while (stripe < v.draw_end_x)
    {
        // 计算贴图 X 坐标：(当前像素 - 绘制起点) * 贴图宽度 / 绘制总宽度
        tex_x = (int)((stripe - v.draw_start_x) * tex->width / v.sprite_w);

        // Z-Buffer 检查：确保精灵不会穿过墙壁
        if (stripe >= 0 && stripe < WIDTH && trans_y > 0 && trans_y < game->z_buffer[stripe])
        {
            y = v.draw_start_y;
            while (y < v.draw_end_y)
            {
                // 计算贴图 Y 坐标：(当前像素 - 绘制起点) * 贴图高度 / 绘制总高度
                tex_y = (int)((y - v.draw_start_y) * tex->height / v.sprite_h);

                // 安全检查，防止浮点误差导致越界
                if (tex_x >= 0 && tex_x < tex->width && tex_y >= 0 && tex_y < tex->height)
                {
                    // 获取像素颜色
                    color = *(int *)(tex->data + (tex_y * tex->size_line + tex_x * (tex->bpp / 8)));

                    // 0x00FFFFFF 过滤透明色 (XPM 的 None)
                    if ((color & 0x00FFFFFF) != 0)
                    {
                        // 2. 应用阴影 (Shading)
                        // 分别提取 R, G, B 通道并乘以亮度系数
                        int r = ((color >> 16) & 0xFF) * shading;
                        int g = ((color >> 8) & 0xFF) * shading;
                        int b = (color & 0xFF) * shading;
                        color = (r << 16) | (g << 8) | b;

                        put_pixel(stripe, y, color, game);
                    }
                }
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

    // 1. 选择贴图
    tex = (conf->is_directional) ? 
          &conf->frames[get_sprite_dir_index(game, s)] : &conf->frames[s->cur_frame];

    // 2. 尺寸缩放
    v.sprite_h = abs((int)(HEIGHT / t_y)) / conf->v_div;
    v.sprite_w = abs((int)(HEIGHT / t_y)) / conf->h_div;

    // 3. 关键：垂直偏移补偿
    // 增加 v_offset 的逻辑：(垂直位移系数 / 距离)
    // 这里的 conf->v_move 建议设为 300.0 到 500.0 之间的正数
    v.v_offset = (int)(conf->v_move / t_y);

    // 4. 计算 Y 轴渲染边界
    // HEIGHT / 2 是地平线。我们把中心点向下推 v_offset。
    v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    v.draw_end_y = v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    
    // Clamping 边界裁剪
    if (v.draw_start_y < 0) v.draw_start_y = 0;
    if (v.draw_end_y >= HEIGHT) v.draw_end_y = HEIGHT - 1;

    // 5. 计算 X 轴
    v.screen_x = (int)((WIDTH / 2) * (1 + t_x / t_y));
    v.draw_start_x = -v.sprite_w / 2 + v.screen_x;
    v.draw_end_x = v.sprite_w / 2 + v.screen_x;

    if (v.draw_start_x < 0) v.draw_start_x = 0;
    if (v.draw_end_x >= WIDTH) v.draw_end_x = WIDTH - 1;

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
