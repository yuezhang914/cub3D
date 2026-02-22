/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:31:54 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:43:58 by yzhang2          ###   ########.fr       */
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

void	enable_mouse(t_game *game)
{
	if (game == NULL || game->mlx == NULL || game->win == NULL)
		return ;
	if (game->player.mouse_enabled == 0)
		return ;
	mlx_mouse_hide(game->mlx, game->win);
	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
}

int	on_mouse_move(int x, int y, t_game *game)
{
	int		dx;
	float	delta;

	(void)y;
	if (game == NULL)
		return (0);
	if (game->player.mouse_enabled == 0)
		return (0);
	if (x == WIDTH / 2)
		return (0);
	dx = x - (WIDTH / 2);
	delta = (float)dx * game->player.mouse_sens;
	game->player.angle = normalize_angle(game->player.angle + delta);
	mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
