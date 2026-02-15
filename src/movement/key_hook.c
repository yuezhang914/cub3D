/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:01:09 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/09 20:01:20 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int	key_press(int keycode, void *param)
{
	t_player	*player;

	player = (t_player *)param;
	if (keycode == W)
		player->key_up = true;
	else if (keycode == S)
		player->key_down = true;
	else if (keycode == A)
		player->key_left = true;
	else if (keycode == D)
		player->key_right = true;
	else if (keycode == LEFT)
		player->left_rotate = true;
	else if (keycode == RIGHT)
		player->right_rotate = true;
	else if (keycode == ESC)
		exit(0);
	return (0);
}

int	key_release(int keycode, void *param)
{
	t_player	*player;

	player = (t_player *)param;
	if (keycode == W)
		player->key_up = false;
	else if (keycode == S)
		player->key_down = false;
	else if (keycode == A)
		player->key_left = false;
	else if (keycode == D)
		player->key_right = false;
	else if (keycode == LEFT)
		player->left_rotate = false;
	else if (keycode == RIGHT)
		player->right_rotate = false;
	return (0);
}
