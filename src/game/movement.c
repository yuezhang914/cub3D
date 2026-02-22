/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 10:13:36 by weiyang          #+#    #+#             */
/*   Updated: 2026/02/17 20:48:15 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	tile_blocks(t_game *game, int x, int y)
{
	char	c;

	if (x < 0 || y < 0 || x >= game->map_w || y >= game->map_h)
		return (true);
	c = game->map[y][x];
	if (c == '1' || c == ' ')
		return (true);
	if (c == 'D')
	{
		if (game->door_state && game->door_state[y][x] == 0)
			return (true);
	}
	return (false);
}

static bool	blocked_with_buffer(t_game *game, float nx, float ny)
{
	int	x0;
	int	x1;
	int	y0;
	int	y1;

	if (nx < 0.0f || ny < 0.0f)
		return (true);
	if (nx >= (float)game->map_w || ny >= (float)game->map_h)
		return (true);
	x0 = (int)(nx - WALL_BUFFER);
	x1 = (int)(nx + WALL_BUFFER);
	y0 = (int)(ny - WALL_BUFFER);
	y1 = (int)(ny + WALL_BUFFER);
	if (tile_blocks(game, x0, y0))
		return (true);
	if (tile_blocks(game, x1, y0))
		return (true);
	if (tile_blocks(game, x0, y1))
		return (true);
	if (tile_blocks(game, x1, y1))
		return (true);
	return (false);
}

static void	try_move_axis(t_game *game, float dx, float dy)
{
	float	nx;
	float	ny;

	nx = game->player.x + dx;
	ny = game->player.y;
	if (!blocked_with_buffer(game, nx, ny))
		game->player.x = nx;
	nx = game->player.x;
	ny = game->player.y + dy;
	if (!blocked_with_buffer(game, nx, ny))
		game->player.y = ny;
}

static void	get_move_offset(t_game *game, float *dx, float *dy)
{
	float	ca;
	float	sa;

	ca = cosf(game->player.angle) * game->player.move_speed;
	sa = sinf(game->player.angle) * game->player.move_speed;
	if (game->player.key_up)
	{
		*dx += ca;
		*dy += sa;
	}
	if (game->player.key_down)
	{
		*dx -= ca;
		*dy -= sa;
	}
	if (game->player.key_left)
	{
		*dx -= sa;
		*dy += ca;
	}
	if (game->player.key_right)
	{
		*dx += sa;
		*dy -= ca;
	}
}

void	update_player(t_game *game)
{
	float	dx;
	float	dy;

	if (game->player.key_rot_l)
		game->player.angle -= game->player.rot_speed;
	if (game->player.key_rot_r)
		game->player.angle += game->player.rot_speed;
	while (game->player.angle >= 2.0f * PI)
		game->player.angle -= 2.0f * PI;
	while (game->player.angle < 0.0f)
		game->player.angle += 2.0f * PI;
	dx = 0;
	dy = 0;
	get_move_offset(game, &dx, &dy);
	if (dx != 0.0f || dy != 0.0f)
		try_move_axis(game, dx, dy);
}
