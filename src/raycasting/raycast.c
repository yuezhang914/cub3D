/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:01:36 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/09 20:01:43 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * MLX 渲染循环：处理清屏、绘图、位置更新
 */
int draw_loop(t_game *game)
{
	t_player *player = &game->player;

	// 1. 获取玩家方向向量 (单位向量)
	float dir_x = cos(player->angle);
	float dir_y = sin(player->angle);

	// 2. 使用 focal_length 计算视平面向量 (相机平面)
	// 注意：focal_length 是总宽度，所以两边延伸各占一半
	float plane_x = -dir_y * (game->focal_length / 2.0f);
	float plane_y = dir_x * (game->focal_length / 2.0f);

	int i = 0;

	while (i < WIDTH)
	{
		// 3. 计算射线方向 (x 范围从 -1 到 1)
		float camera_x = 2.0f * i / (float)WIDTH - 1.0f;
		float ray_dir_x = dir_x + plane_x * camera_x;
		float ray_dir_y = dir_y + plane_y * camera_x;

		// 4. 调用射线投射 (建议直接传 ray_dir 而不是角度，效率更高)
		draw_line(game, ray_dir_x, ray_dir_y, i);
		i++;
	}
	// 5. 更新位置与小地图
	move_player(player, game);
	render_minimap(game); // 使用你定义的 pix_per_unit 绘制

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
