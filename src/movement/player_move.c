/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:18:10 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/15 15:18:12 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	handle_rotation(t_player *player)
{
	if (player->left_rotate)
		player->angle -= player->rotate_speed;
	if (player->right_rotate)
		player->angle += player->rotate_speed;
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	else if (player->angle < 0)
		player->angle += 2 * PI;
}

static void	calculate_move(t_player *p, float *move_x, float *move_y)
{
	float	ca;
	float	sa;

	ca = cos(p->angle);
	sa = sin(p->angle);
	if (p->key_up)
	{
		*move_x += ca * p->move_speed;
		*move_y += sa * p->move_speed;
	}
	if (p->key_down)
	{
		*move_x -= ca * p->move_speed;
		*move_y -= sa * p->move_speed;
	}
	if (p->key_left)
	{
		*move_x += sa * p->move_speed;
		*move_y -= ca * p->move_speed;
	}
	if (p->key_right)
	{
		*move_x -= sa * p->move_speed;
		*move_y += ca * p->move_speed;
	}
}

static bool	touch(float px, float py, t_game *game)
{
	int	x;
	int	y;

	if (px < 0 || py < 0)
		return (true);
	x = (int)px;
	y = (int)py;
	if (y >= game->map_h || x >= game->map_w)
		return (true);
	if (game->map[y][x] == '1' || game->map[y][x] == ' ')
		return (true);
	return (false);
}

static void	apply_movement(t_player *p, t_game *g, float mx, float my)
{
	float	bx;
	float	by;

	bx = 0.1f;
	if (mx < 0)
		bx = -0.1f;
	by = 0.1f;
	if (my < 0)
		by = -0.1f;
	if (!touch(p->x + mx + bx, p->y, g))
		p->x += mx;
	if (!touch(p->x, p->y + my + by, g))
		p->y += my;
	p->map_x = (int)p->x;
	p->map_y = (int)p->y;
}

void	move_player(t_player *player, t_game *game)
{
	float	move_x;
	float	move_y;

	move_x = 0;
	move_y = 0;
	handle_rotation(player);
	calculate_move(player, &move_x, &move_y);
	apply_movement(player, game, move_x, move_y);
}
