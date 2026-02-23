/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 13:04:44 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

static int	is_hit_bonus(t_game *game, t_dda *d, float rx, float ry)
{
	char	tile;

	tile = game->map[d->map_y][d->map_x];
	if (tile == '1')
		return (1);
	if (tile == 'D')
	{
		if (door_ray_blocks(game, d, rx, ry))
			return (1);
	}
	return (0);
}
#else

static int	is_hit_bonus(t_game *game, t_dda *d, float rx, float ry)
{
	(void)rx;
	(void)ry;
	if (game->map[d->map_y][d->map_x] == '1')
		return (1);
	return (0);
}
#endif

int	ray_run_dda_until_hit(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	while (1)
	{
		ray_step_once(d);
		if (ray_outside_map(game, d))
			return (0);
		if (is_hit_bonus(game, d, r_dir_x, r_dir_y))
			return (1);
	}
	return (0);
}

t_render_vars	ray_make_render_vars(t_game *game, t_dda *d, float r_dir_x,
		float r_dir_y)
{
	t_render_vars	v;
	t_ray			ray;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = ray_pick_texture(game, d, r_dir_x, r_dir_y);
	v.perp_dist = ray_get_perp_dist(d);
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;
	ray.perp_dist = v.perp_dist;
	ray.dir_x = r_dir_x;
	ray.dir_y = r_dir_y;
	v.line_h = (int)(HEIGHT / v.perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);
	v.tex_x = ray_calc_tex_x(game, d, v.tex, ray);
	return (v);
}
