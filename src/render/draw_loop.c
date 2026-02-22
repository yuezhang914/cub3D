/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_loop.c                                            :+:      :+:    : */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:06:22 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/18 01:45:30 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	setup_camera(t_game *game, float dir[2], float plane[2])
{
	dir[0] = cos(game->player.angle);
	dir[1] = sin(game->player.angle);
	plane[0] = -dir[1] * (game->focal_length / 2.0f);
	plane[1] = dir[0] * (game->focal_length / 2.0f);
}

void	draw_loop(t_game *game)
{
	float		dir[2];
	float		plane[2];
	int			x;
	float		cam_x;
	t_coords	ray_dir;

	setup_camera(game, dir, plane);
	x = 0;
	while (x < WIDTH)
	{
		cam_x = 2.0f * x / (float)WIDTH - 1.0f;
		ray_dir.x = dir[0] + plane[0] * cam_x;
		ray_dir.y = dir[1] + plane[1] * cam_x;
		render_column(game, ray_dir.x, ray_dir.y, x);
		x++;
	}
}

void	clear_image(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}
