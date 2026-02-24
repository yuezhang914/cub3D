/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 17:28:05 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

static void	handle_interact_key(t_game *game)
{
	try_toggle_door(game);
}

#else

static void	handle_interact_key(t_game *game)
{
	(void)game;
}

#endif

int	on_key_down(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		graceful_exit(game, 0, NULL, NULL);
	else if (keycode == XK_w)
		game->player.key_up = 1;
	else if (keycode == XK_s)
		game->player.key_down = 1;
	else if (keycode == XK_d)
		game->player.key_left = 1;
	else if (keycode == XK_a)
		game->player.key_right = 1;
	else if (keycode == XK_Left)
		game->player.key_rot_l = 1;
	else if (keycode == XK_Right)
		game->player.key_rot_r = 1;
	else if (keycode == XK_e)
		handle_interact_key(game);
	return (0);
}

int	on_key_up(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->player.key_up = 0;
	else if (keycode == XK_s)
		game->player.key_down = 0;
	else if (keycode == XK_a)
		game->player.key_left = 0;
	else if (keycode == XK_d)
		game->player.key_right = 0;
	else if (keycode == XK_Left)
		game->player.key_rot_l = 0;
	else if (keycode == XK_Right)
		game->player.key_rot_r = 0;
	return (0);
}
