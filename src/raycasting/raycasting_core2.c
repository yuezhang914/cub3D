/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core 2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:58:02 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ray_step_once(t_dda *d)
{
	if (d->side_x < d->side_y)
	{
		d->side_x += d->delta_x;
		d->map_x += d->step_x;
		d->side = 0;
		return ;
	}
	d->side_y += d->delta_y;
	d->map_y += d->step_y;
	d->side = 1;
}

int	ray_outside_map(t_game *game, t_dda *d)
{
	if (d->map_x < 0 || d->map_x >= game->map_w)
		return (1);
	if (d->map_y < 0 || d->map_y >= game->map_h)
		return (1);
	return (0);
}

t_tex	*ray_pick_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
	if (game->map[d->map_y][d->map_x] == 'D')
	{
		if (game->door_state && game->door_state[d->map_y][d->map_x] == 1)
			return (&game->door_open);
		return (&game->door);
	}
	if (d->side == 0 && r_dx > 0.0f)
		return (&game->east);
	if (d->side == 0)
		return (&game->west);
	if (r_dy > 0.0f)
		return (&game->south);
	return (&game->north);
}

float	ray_get_perp_dist(t_dda *d)
{
	if (d->side == 0)
		return (d->side_x - d->delta_x);
	return (d->side_y - d->delta_y);
}

int	ray_calc_tex_x(t_game *game, t_dda *d, t_tex *tex, t_ray ray)
{
	float	hit;
	int		tex_x;

	if (d->side == 0)
		hit = game->player.y + ray.perp_dist * ray.dir_y;
	else
		hit = game->player.x + ray.perp_dist * ray.dir_x;
	hit -= floor((double)hit);
	if (hit < 0)
		hit += 1.0;
	tex_x = (int)(hit * (float)tex->width);
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_x < 0)
		tex_x = 0;
	if ((d->side == 0 && ray.dir_x < 0.0) || (d->side == 1 && ray.dir_y > 0.0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}
