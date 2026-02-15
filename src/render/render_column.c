/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:56 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:01:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
	game->data[index + 3] = 0;
}

static void	draw_wall(t_render_vars v, t_game *game, float step, float tex_pos)
{
	int	y;
	int	color;
	int	tex_y;

	y = v.start;
	if (y < 0)
		y = 0;
	while (y <= v.end && y < HEIGHT)
	{
		tex_y = (int)tex_pos % v.tex->height;
		color = *(int *)(v.tex->data + (tex_y * v.tex->size_line
					+ v.tex_x * (v.tex->bpp / 8)));
		put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
}

void	render_column(t_render_vars v, t_game *game)
{
	int		y;
	float	step;
	float	tex_pos;

	y = -1;
	while (++y < v.start && y < HEIGHT)
		put_pixel(v.x, y, game->ceiling_color, game);
	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = 0;
	if (v.start < 0)
		tex_pos = (float)-v.start * step;
	draw_wall(v, game, step, tex_pos);
	y = v.end;
	if (y < 0)
		y = -1;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}
