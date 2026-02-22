/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>               +#+  +:+ 	+#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:56 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:01:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

void	draw_door_overlay_column_bonus(t_game *game, float rx, float ry, int i);
#else

void	draw_door_overlay_column_bonus(t_game *game, float rx, float ry, int i)
{
	(void)game;
	(void)rx;
	(void)ry;
	(void)i;
}
#endif

static void	draw_ceiling_part(t_game *game, t_render_vars v)
{
	int	y;

	y = 0;
	while (y < v.start && y < HEIGHT)
	{
		put_pixel(v.x, y, game->ceiling_color, game);
		y++;
	}
}

static void	draw_floor_part(t_game *game, t_render_vars v)
{
	int	y;

	y = v.end;
	if (y < 0)
		y = -1;
	while (++y < HEIGHT)
		put_pixel(v.x, y, game->floor_color, game);
}

static void	draw_wall_part(t_game *game, t_render_vars v)
{
	float	step;
	float	tex_pos;

	step = 1.0f * v.tex->height / v.line_h;
	tex_pos = 0.0f;
	if (v.start < 0)
		tex_pos = (float)(-v.start)*step;
	draw_wall(v, game, step, tex_pos);
}

void	render_column(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_render_vars	v;

	v = get_render_vars(game, r_dir_x, r_dir_y, i);
	draw_ceiling_part(game, v);
	draw_wall_part(game, v);
	draw_floor_part(game, v);
	draw_door_overlay_column_bonus(game, r_dir_x, r_dir_y, i);
}
