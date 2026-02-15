/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:01:36 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/09 20:01:43 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/**
 * 计算相机方向向量和视平面向量
 */
static void	setup_camera(t_game *game, float dir[2], float plane[2])
{
	dir[0] = cos(game->player.angle);
	dir[1] = sin(game->player.angle);
	plane[0] = -dir[1] * (game->focal_length / 2.0f);
	plane[1] = dir[0] * (game->focal_length / 2.0f);
}

/**
 * 遍历屏幕宽度，为每一列发射射线
 */
static void	render_rays(t_game *game, float dir[2], float plane[2])
{
	int		i;
	float	cam_x;
	float	ray_dir_x;
	float	ray_dir_y;

	i = 0;
	while (i < WIDTH)
	{
		cam_x = 2.0f * i / (float)WIDTH - 1.0f;
		ray_dir_x = dir[0] + plane[0] * cam_x;
		ray_dir_y = dir[1] + plane[1] * cam_x;
		draw_line(game, ray_dir_x, ray_dir_y, i);
		i++;
	}
}

/**
 * 游戏主渲染循环
 * 作用：计算视平面、渲染 3D 场景、处理物理移动、叠加小地图并显示。
 */
int	draw_loop(t_game *game)
{
	float	dir[2];
	float	plane[2];

	setup_camera(game, dir, plane);
	render_rays(game, dir, plane);
	move_player(&game->player, game);
	render_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

