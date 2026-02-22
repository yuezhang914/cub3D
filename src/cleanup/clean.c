/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:00:44 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:32:17 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_str(int fd, const char *s)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	print_error(const char *func, const char *message)
{
	put_str(2, "Error\n");
	if (func && *func)
		put_str(2, func);
	else
		put_str(2, "unknown");
	put_str(2, ": ");
	if (message && *message)
		put_str(2, message);
	else
		put_str(2, "unknown error");
	put_str(2, "\n");
}

void	destroy_images(t_game *game)
{
	t_img	*current;
	t_img	*next;

	if (!game || !game->img_head)
		return ;
	current = game->img_head;
	while (current)
	{
		next = current->next;
		if (game->mlx && current->ptr)
		{
			mlx_destroy_image(game->mlx, current->ptr);
			current->ptr = NULL;
		}
		current = next;
	}
	game->img_head = NULL;
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
		const char *msg)
{
	destroy_images(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
	if (exit_code)
		print_error(func, msg);
	track_clean(game);
	exit(exit_code);
}
