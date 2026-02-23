/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_ray_fov.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:49:57 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/23 14:50:01 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_mini_vision_line(t_game *game)
{
	t_int_xy	c;
	t_coords	ray;
	float		dist;
	int			color;

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;
	dist = 0;
	while (dist < MINI_WIDTH / 2)
	{
		ray.x = c.x + cos(game->player.angle) * dist;
		ray.y = c.y + sin(game->player.angle) * dist;
		color = get_mini_color(game, (int)ray.x, (int)ray.y);
		if (color == 0xFFFFFF || color == 0x1A1A1A)
			break ;
		put_mini_pixel(game, (int)ray.x, (int)ray.y, 0x55FF55);
		dist += 1.0f;
	}
}

static void	draw_vision_ray(t_game *game, float angle)
{
	t_int_xy	c;
	float		dist;
	int			rx;
	int			ry;

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;
	dist = 0;
	while (dist < 30)
	{
		rx = c.x + cos(angle) * dist;
		ry = c.y + sin(angle) * dist;
		if (get_mini_color(game, rx, ry) == 0xFFFFFF)
			break ;
		put_mini_pixel(game, rx, ry, 0x3300FF00);
		dist += 1.0f;
	}
}

void	draw_mini_fov(t_game *game)
{
	float	start_angle;
	float	end_angle;
	float	curr_angle;

	start_angle = game->player.angle - (PI / 6);
	end_angle = game->player.angle + (PI / 6);
	curr_angle = start_angle;
	while (curr_angle <= end_angle)
	{
		draw_vision_ray(game, curr_angle);
		curr_angle += (PI / 90);
	}
}
