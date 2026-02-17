/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:00:44 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 20:57:49 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

/*
** 函数：graceful_exit
** 作用：统一释放资源并退出；exit_code!=0 时打印错误信息
** 参数：
**   game：总结构体
**   exit_code：退出码（0 正常，非 0 错误）
**   func：出错函数名（可为 NULL）
**   msg ：错误信息（可为 NULL）
*/
void	graceful_exit(t_game *game, int exit_code, const char *func,
		const char *msg)
{
	destroy_images(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (exit_code)
		print_error(func, msg);
	track_clean(game);
	exit(exit_code);
}
