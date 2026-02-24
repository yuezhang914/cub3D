/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 14:27:27 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static float	normalize_angle(float a)
{
	while (a > PI)
		a -= 2.0f * PI;
	while (a < -PI)
		a += 2.0f * PI;
	return (a);
}

int	on_mouse_move(int x, int y, t_game *game)
{
	static int	last_x = -1;
	int			dx;
	float		delta;

	(void)y;
	if (game == NULL)
		return (0);
	if (game->player.mouse_enabled == 0)
		return (0);
	if (last_x == -1)
	{
		last_x = x;
		return (0);
	}
	dx = x - last_x;
	last_x = x;
	if (dx > -1 && dx < 1)
		return (0);
	if (dx > 60)
		dx = 60;
	if (dx < -60)
		dx = -60;
	delta = (float)dx * game->player.mouse_sens;
	game->player.angle = normalize_angle(game->player.angle + delta);
	return (0);
}
