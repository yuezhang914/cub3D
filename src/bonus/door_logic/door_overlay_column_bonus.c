/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_overlay_column_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:37:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:54:04 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_overlay_transparent(t_render_vars v, t_game *game, float step,
		float tex_pos)
{
	int	y;
	int	tex_y;
	int	color;

	y = v.start;
	if (y < 0)
		y = 0;
	while (y <= v.end && y < HEIGHT)
	{
		tex_y = (int)tex_pos % v.tex->height;
		color = *(int *)(v.tex->data + (tex_y * v.tex->size_line + v.tex_x
					* (v.tex->bpp / 8)));
		if ((unsigned int)color != 0x00000000u
			&& (unsigned int)color != 0xFF000000u)
			put_pixel(v.x, y, color, game);
		tex_pos += step;
		y++;
	}
}

void	draw_door_overlay_column_bonus(t_game *game, float r_dir_x,
		float r_dir_y, int i)
{
	t_render_vars	door_v;
	float			step;
	float			tex_pos;

	door_v = get_open_door_overlay_vars(game, r_dir_x, r_dir_y, i);
	if (!door_v.tex || !door_v.tex->data)
		return ;
	if (door_v.perp_dist >= game->z_buffer[i])
		return ;
	step = 1.0f * door_v.tex->height / door_v.line_h;
	tex_pos = 0.0f;
	if (door_v.start < 0)
		tex_pos = (float)(-door_v.start)*step;
	draw_overlay_transparent(door_v, game, step, tex_pos);
}
