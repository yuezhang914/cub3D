/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_view_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 23:10:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:29:53 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	load_door_texture(t_game *game)
{
	if (!game)
		return ;
	game->door.path = "assets/door/door_close.xpm";
	load_texture(game, &game->door);
	game->door_open.path = "assets/door/door_open.xpm";
	load_texture(game, &game->door_open);
}

int	door_ray_blocks(t_game *game, t_dda *d, float rdx, float rdy)
{
	float	open;
	float	wall_x;

	if (!game || !game->door_state)
		return (1);
	open = (float)game->door_state[d->map_y][d->map_x];
	if (open <= 0.0f)
		return (1);
	if (open >= 1.0f)
		return (0);
	if (d->side == 0)
		wall_x = game->player.y + d->perp_dist * rdy;
	else
		wall_x = game->player.x + d->perp_dist * rdx;
	wall_x = wall_x - floorf(wall_x);
	if (wall_x < open)
		return (0);
	return (1);
}
