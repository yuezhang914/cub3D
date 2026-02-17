/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:06:22 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/*
** 函数：setup_camera（static）
** 作用：根据玩家视角角度，计算相机方向向量 dir 和视平面向量 plane。
** 参数：
**   game：提供 player.angle 与 focal_length
**   dir：输出二维向量，dir[0]=x，dir[1]=y
**   plane：输出二维向量（与 dir 垂直），用于生成不同列的射线方向
** 说明：
**   这里采用和队友 DDA 渲染一致的相机模型：
**   ray_dir = dir + plane * camera_x，其中 camera_x ∈ [-1, 1]
*/
static void	setup_camera(t_game *game, float dir[2], float plane[2])
{
	dir[0] = cos(game->player.angle);
	dir[1] = sin(game->player.angle);
	plane[0] = -dir[1] * (game->focal_length / 2.0f);
	plane[1] = dir[0] * (game->focal_length / 2.0f);
}

/*
** 函数：draw_walls
** 作用：渲染 3D 墙体（每列发射一条射线），把结果画到 game->img 的帧缓冲里。
** 参数：
**   game：总结构体（提供 player、map、贴图、帧缓冲 data）
** 用在哪：
**   game_step() 每帧调用一次（清屏之后、put 到窗口之前）。
** 依赖：
**   - get_render_vars()：DDA 算法计算命中墙面、距离、纹理坐标等
**   - render_column()：根据 render_vars 把这一列的天花板/墙贴图/地板画出来
*/
void	draw_walls(t_game *game)
{
	float	dir[2];
	float	plane[2];
	int		x;
	float	cam_x;
	float	ray_dir_x;
	float	ray_dir_y;

	setup_camera(game, dir, plane);
	x = 0;
	while (x < WIDTH)
	{
		cam_x = 2.0f * x / (float)WIDTH - 1.0f;
		ray_dir_x = dir[0] + plane[0] * cam_x;
		ray_dir_y = dir[1] + plane[1] * cam_x;
		render_column(game, ray_dir_x, ray_dir_y, x);
		x++;
	}
}
