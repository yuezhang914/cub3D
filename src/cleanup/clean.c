/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:00:44 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 12:25:17 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	destroy_images(t_game *game)
{
	t_img	*current;

	current = game->img_head;
	while (current)
	{
		mlx_destroy_image(game->mlx, current->ptr);
		current = current->next;
	}
}

void	track_clean(t_game *game)
{
	t_gnode	*tmp;
	t_gnode	*current;

	current = game->track_head;
	while (current)
	{
		if (current->ptr)
			free(current->ptr);
		tmp = current;
		current = current->next;
		free(tmp);
	}
	game->track_head = NULL;
}

void	graceful_exit(t_game *game, int exit_code, const char *func,
		char *message)
{
	destroy_images(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (exit_code)
		print_error(func, message);
	track_clean(game);
	exit(exit_code);
}
