/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_logic_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:28:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:44:18 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	door_is_closed(t_game *game, int x, int y)
{
	if (!game || !game->door_state || !game->map)
		return (0);
	if (x < 0 || y < 0 || x >= game->map_w || y >= game->map_h)
		return (0);
	if (game->map[y][x] != 'D')
		return (0);
	return (game->door_state[y][x] <= 0.0f);
}

void	init_doors(t_game *game)
{
	int	y;

	if (!game || !game->map)
		return ;
	game->door_state = (unsigned char **)track_malloc(game,
			sizeof(unsigned char *) * game->map_h);
	y = 0;
	while (y < game->map_h)
	{
		game->door_state[y] = (unsigned char *)track_malloc(game,
				sizeof(unsigned char) * game->map_w);
		ft_bzero(game->door_state[y], (size_t)game->map_w);
		y++;
	}
}

static t_int_xy	door_target_tile(t_game *game)
{
	t_int_xy	pos;
	float		rx;
	float		ry;

	rx = game->player.x + cosf(game->player.angle) * 0.75f;
	ry = game->player.y + sinf(game->player.angle) * 0.75f;
	pos.x = (int)rx;
	pos.y = (int)ry;
	return (pos);
}

void	try_toggle_door(t_game *game)
{
	t_int_xy	p;

	if (!game || !game->door_state)
		return ;
	p = door_target_tile(game);
	if (p.x < 0 || p.y < 0 || p.x >= game->map_w || p.y >= game->map_h)
		return ;
	if (game->map[p.y][p.x] != 'D')
		return ;
	game->door_state[p.y][p.x] = (unsigned char)(!game->door_state[p.y][p.x]);
}
