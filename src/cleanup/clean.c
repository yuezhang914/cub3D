/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:00:44 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 21:46:28 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：destroy_images
** 作用：遍历 img_head 链表，逐个 mlx_destroy_image
*/
void	destroy_images(t_game *game)
{
	t_img	*current;
	t_img	*next;

	if (!game)
		return ;
	current = game->img_head;
	while (current)
	{
		next = current->next;
		if (game->mlx && current->ptr)
			mlx_destroy_image(game->mlx, current->ptr);
		current->ptr = NULL;
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
	/* ✅ 新增：释放 X11 display（Linux 版 minilibx 需要） */
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
