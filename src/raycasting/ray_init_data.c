/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_init_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 18:10:16 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/22 18:10:18 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ray_init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;
	if (r_dir_x == 0.0f)
		d->delta_x = (float)INF;
	else
		d->delta_x = fabsf(1.0f / r_dir_x);
	if (r_dir_y == 0.0f)
		d->delta_y = (float)INF;
	else
		d->delta_y = fabsf(1.0f / r_dir_y);
	ray_set_step_x(game, d, r_dir_x);
	ray_set_step_y(game, d, r_dir_y);
}

void	ray_set_step_x(t_game *game, t_dda *d, float r_dir_x)
{
	if (r_dir_x < 0.0f)
	{
		d->step_x = -1;
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
		return ;
	}
	d->step_x = 1;
	d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
}

void	ray_set_step_y(t_game *game, t_dda *d, float r_dir_y)
{
	if (r_dir_y < 0.0f)
	{
		d->step_y = -1;
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
		return ;
	}
	d->step_y = 1;
	d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
}
