/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_dda.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 18:01:23 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 18:01:32 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	init_side_dist(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	if (r_dir_x < 0)
	{
		d->step_x = -1;
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
	}
	else
	{
		d->step_x = 1;
		d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
	}
	if (r_dir_y < 0)
	{
		d->step_y = -1;
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
	}
	else
	{
		d->step_y = 1;
		d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
	}
}

void	init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;
	d->delta_x = 1e30;
	if (r_dir_x != 0)
		d->delta_x = fabsf(1.0f / r_dir_x);
	d->delta_y = 1e30;
	if (r_dir_y != 0)
		d->delta_y = fabsf(1.0f / r_dir_y);
	init_side_dist(game, d, r_dir_x, r_dir_y);
}

int	perform_dda(t_game *game, t_dda *d)
{
	while (1)
	{
		if (d->side_x < d->side_y)
		{
			d->side_x += d->delta_x;
			d->map_x += d->step_x;
			d->side = 0;
		}
		else
		{
			d->side_y += d->delta_y;
			d->map_y += d->step_y;
			d->side = 1;
		}
		if (d->map_x < 0 || d->map_x >= game->map_w \
			|| d->map_y < 0 || d->map_y >= game->map_h)
			return (0);
		if (game->map[d->map_y][d->map_x] == '1')
			break ;
	}
	return (1);
}
