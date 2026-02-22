/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:55:20 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_render_vars	get_render_vars(t_game *game, float r_dir_x, float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	ray_init_dda(game, &d, r_dir_x, r_dir_y);
	if (!ray_run_dda_until_hit(game, &d, r_dir_x, r_dir_y))
		return (v);
	v = ray_make_render_vars(game, &d, r_dir_x, r_dir_y);
	game->z_buffer[i] = v.perp_dist;
	v.x = i;
	return (v);
}
