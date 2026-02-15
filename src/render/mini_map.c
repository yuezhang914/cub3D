/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:00:39 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	put_mini_pixel(t_game *game, int x, int y, int color)
{
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return ;
	put_pixel(x + 20, y + 20, color, game);
}

static void	draw_mini_player(t_game *game)
{
	int	center_x;
	int	center_y;
	int	i;
	int	p[2];

	center_x = MINI_WIDTH / 2;
	center_y = MINI_HEIGHT / 2;
	p[1] = -3;
	while (++p[1] <= 2)
	{
		p[0] = -3;
		while (++p[0] <= 2)
			put_mini_pixel(game, center_x + p[0], center_y + p[1], 0xFF0000);
	}
	i = -1;
	while (++i < 12)
	{
		p[0] = center_x + (int)(cos(game->player.angle) * i);
		p[1] = center_y + (int)(sin(game->player.angle) * i);
		put_mini_pixel(game, p[0], p[1], 0x00FF00);
	}
}

static int	get_mini_color(t_game *game, int x, int y)
{
	float	map_x;
	float	map_y;

	map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
	map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;
	if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
		return (0x1A1A1A);
	if (game->map[(int)map_y][(int)map_x] == '1')
		return (0xFFFFFF);
	return (0x333333);
}

void	render_minimap(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < MINI_HEIGHT)
	{
		x = 0;
		while (x < MINI_WIDTH)
		{
			put_mini_pixel(game, x, y, get_mini_color(game, x, y));
			x++;
		}
		y++;
	}
	draw_mini_player(game);
}
