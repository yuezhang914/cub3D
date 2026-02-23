/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:37 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 20:27:03 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_mini_pixel(t_game *game, int x, int y, int color)
{
	if (x < 0 || x >= MINI_WIDTH || y < 0 || y >= MINI_HEIGHT)
		return ;
	put_pixel(x + 20, y + 20, color, game);
}

static void	draw_mini_player(t_game *game)
{
	t_int_xy	p;
	t_int_xy	c;
	int			i;

	c.x = MINI_WIDTH / 2;
	c.y = MINI_HEIGHT / 2;
	p.y = -3;
	while (++p.y <= 2)
	{
		p.x = -3;
		while (++p.x <= 2)
			put_mini_pixel(game, c.x + p.x, c.y + p.y, 0xFF0000);
	}
	i = -1;
	while (++i < 12)
	{
		put_mini_pixel(game, c.x + (int)(cos(game->player.angle) * i), c.y
			+ (int)(sin(game->player.angle) * i), 0x00FF00);
	}
}

static int	get_mini_color(t_game *game, int x, int y)
{
	float	map_x;
	float	map_y;
	char	c;

	map_x = game->player.x + (float)(x - MINI_WIDTH / 2) / game->pix_per_unit;
	map_y = game->player.y + (float)(y - MINI_HEIGHT / 2) / game->pix_per_unit;
	if (map_x < 0 || map_y < 0 || map_x >= game->map_w || map_y >= game->map_h)
		return (0x1A1A1A);
	c = game->map[(int)map_y][(int)map_x];
	if (c == '1')
		return (0xFFFFFF);
	if (c == 'D')
	{
		if (game->door_state && game->door_state[(int)map_y][(int)map_x] == 0)
			return (0xAAAAAA);
		return (0x333333);
	}
	return (0x333333);
}

static void	draw_mini_sprites(t_game *game)
{
	int			i;
	t_int_xy	s;
	t_int_xy	d2;
	t_coords	offset;

	i = 0;
	while (i < game->sprs.num)
	{
		offset.x = game->sprs.list[i].x - game->player.x;
		offset.y = game->sprs.list[i].y - game->player.y;
		s.x = (int)(MINI_WIDTH / 2 + offset.x * game->pix_per_unit);
		s.y = (int)(MINI_HEIGHT / 2 + offset.y * game->pix_per_unit);
		d2.y = -2;
		while (++d2.y <= 1)
		{
			d2.x = -2;
			while (++d2.x <= 1)
				put_mini_pixel(game, s.x + d2.x, s.y + d2.y, 0x00FF00);
		}
		i++;
	}
}

void	render_minimap(t_game *game)
{
	t_int_xy	p;

	p.y = 0;
	while (p.y < MINI_HEIGHT)
	{
		p.x = 0;
		while (p.x < MINI_WIDTH)
		{
			put_mini_pixel(game, p.x, p.y, get_mini_color(game, p.x, p.y));
			p.x++;
		}
		p.y++;
	}
	draw_mini_player(game);
	draw_mini_sprites(game);
}
