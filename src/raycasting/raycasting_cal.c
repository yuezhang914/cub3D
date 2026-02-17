/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray_line.c                                        :+:      :+:    :+*/
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:00:22 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static t_tex	*get_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
	if (d->side == 0)
	{
		if (r_dx > 0)
			return (&game->east);
		return (&game->west);
	}
	if (r_dy > 0)
		return (&game->south);
	return (&game->north);
}

static void	set_wall_dist(t_render_vars *v, t_dda *d)
{
	if (d->side == 0)
		d->perp_dist = d->side_x - d->delta_x;
	else
		d->perp_dist = d->side_y - d->delta_y;
	if (d->perp_dist < 0.1f)
		d->perp_dist = 0.1f;
	v->line_h = (int)(HEIGHT / d->perp_dist);
	v->start = (HEIGHT / 2) - (v->line_h / 2);
	v->end = (HEIGHT / 2) + (v->line_h / 2);
}

static void	set_tex_x(t_game *game, t_render_vars *v, t_dda *d, float r_dir[2])
{
	float	wall_hit;

	if (d->side == 0)
		wall_hit = game->player.y + d->perp_dist * r_dir[1];
	else
		wall_hit = game->player.x + d->perp_dist * r_dir[0];
	wall_hit -= floor(wall_hit);
	v->tex_x = (int)(wall_hit * (float)v->tex->width);
	if (v->tex_x >= v->tex->width)
		v->tex_x = v->tex->width - 1;
	if (v->tex_x < 0)
		v->tex_x = 0;
	if ((d->side == 0 && r_dir[0] < 0) || (d->side == 1 && r_dir[1] > 0))
		v->tex_x = v->tex->width - v->tex_x - 1;
}

static t_render_vars	calculate_render_vars(t_game *game, t_dda *d,
						int i, float r_dir[2])
{
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = get_texture(game, d, r_dir[0], r_dir[1]);
	if (!v.tex)
		return (v);
	v.x = i;
	set_wall_dist(&v, d);
	set_tex_x(game, &v, d, r_dir);
	return (v);
}

t_render_vars	get_render_vars(t_game *game, float r_dir_x,
					float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;
	float			r_dir[2];

	ft_bzero(&v, sizeof(t_render_vars));
	r_dir[0] = r_dir_x;
	r_dir[1] = r_dir_y;
	init_dda(game, &d, r_dir_x, r_dir_y);
	if (!perform_dda(game, &d))
		return (v);
	v = calculate_render_vars(game, &d, i, r_dir);
	return (v);
}
