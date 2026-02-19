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

static void draw_sprite_pixels(t_game *game, t_sprite_render_vars v,
                               float trans_y, t_tex *tex) // 👈 增加贴图参数
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
        // 使用传入的 tex->width 替代写死的 game->sprs.tex.width
        tex_x = (int)(256 * (stripe - (-v.sprite_w / 2 + v.screen_x)) * tex->width / v.sprite_w) / 256;
        
        if (trans_y > 0 && stripe >= 0 && stripe < WIDTH && trans_y < game->z_buffer[stripe])
        {
            y = v.draw_start_y;
            while (y < v.draw_end_y)
            {
                // 使用传入的 v.v_move_screen 修正 Y 坐标映射
                d = (y - v.v_offset) * 256 - HEIGHT * 128 + v.sprite_h * 128;
                tex_y = ((d * tex->height) / v.sprite_h) / 256;
                
                // 边界检查，防止 tex_y 越界导致段错误
                if (tex_y >= 0 && tex_y < tex->height && tex_x >= 0 && tex_x < tex->width)
                {
                    color = *(int *)(tex->data + (tex_y * tex->size_line + tex_x * (tex->bpp / 8)));
                    // 0x00FFFFFF 过滤掉透明色（黑色背景通常在 XPM 中为透明）
                    if ((color & 0x00FFFFFF) != 0)
                        put_pixel(stripe, y, color, game);
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

    // 1. 计算从精灵指向玩家的角度 (使用 atan2 计算 y/x 的弧度)
    // 这代表玩家位于精灵的哪个方位
    view_angle = atan2(game->player.y - s->y, game->player.x - s->x);

    // 2. 减去精灵自身的面朝方向 (s->angle) 得到相对角度
    relative_angle = view_angle - s->angle;

    // 3. 将角度标准化到 [0, 2*PI] 之间
    while (relative_angle < 0)
        relative_angle += 2.0f * PI;
    while (relative_angle >= 2.0f * PI)
        relative_angle -= 2.0f * PI;

    // 4. 将 360 度划分为 8 份，每份 45 度 (PI/4)
    // 加上 M_PI/8 (22.5度) 的偏移是为了让“正面”处于 -22.5 到 +22.5 之间
    index = (int)((relative_angle + PI / 8.0f) / (PI / 4.0f)) % 8;

    return (index);
}

static void draw_single_sprite(t_game *game, t_sprite *s, float t_x, float t_y)
{
    t_sprite_render_vars v;
    t_sprite_config      *conf = &game->config[s->type]; // 获取该类型的配置
    t_tex                *tex;

    // 1. 选择正确的贴图帧
    if (conf->is_directional)
        tex = &conf->frames[get_sprite_dir_index(game, s)];
    else
        tex = &conf->frames[s->cur_frame];

    // 2. 计算垂直偏移 (v_move)
    v.v_offset = (int)(conf->v_move / t_y);

    // 3. 计算高度和范围 (应用 v_offset)
    v.sprite_h = abs((int)(HEIGHT / t_y));
    v.draw_start_y = -v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    v.draw_end_y = v.sprite_h / 2 + HEIGHT / 2 + v.v_offset;
    
    // 裁剪 Y 范围防止越界
    if (v.draw_start_y < 0) v.draw_start_y = 0;
    if (v.draw_end_y >= HEIGHT) v.draw_end_y = HEIGHT - 1;

    // 4. 计算宽度和水平范围
    v.sprite_w = abs((int)(HEIGHT / t_y));
    v.screen_x = (int)((WIDTH / 2) * (1 + t_x / t_y));
    v.draw_start_x = -v.sprite_w / 2 + v.screen_x;
    v.draw_end_x = v.sprite_w / 2 + v.screen_x;

    if (v.draw_start_x < 0) v.draw_start_x = 0;
    if (v.draw_end_x >= WIDTH) v.draw_end_x = WIDTH - 1;

    // 5. 执行绘制，传入选中的 tex
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
