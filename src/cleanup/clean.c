/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 22:00:44 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:40:37 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：put_str（static）
** 作用：向指定 fd 输出一个字符串（不自动加换行）
** 参数：
**   fd：文件描述符（2 表示 stderr）
**   s：要输出的字符串，允许 NULL
** 主要逻辑：
**   - s==NULL 就不做事
**   - write(fd, s, ft_strlen(s))
** 调用者：
**   print_error()
*/
static void	put_str(int fd, const char *s)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

/*
** 函数名：print_error
** 作用：以统一格式打印错误信息到 stderr
** 输出格式：
**   Error
**   <func>: <message>
**
** 参数：
**   func：出错函数名（一般传 __func__），允许 NULL/空
**   message：错误描述，允许 NULL/空
**
** 主要逻辑：
**   - 先输出固定前缀 "Error\n"
**   - func 为空就打印 "unknown"
**   - message 为空就打印 "unknown error"
**
** 调用者：
**   graceful_exit(exit_code != 0 时)
*/
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

/*
** 函数名：destroy_images
** 作用：销毁你们用 remember_image() 记录过的所有 mlx image（贴图等）
**
** 参数：
**   game：包含
**     - game->mlx：mlx 句柄
**     - game->img_head：t_img 链表头（每个节点 ptr 是一个 image）
**
** 主要逻辑：
**   - 遍历 img_head 链表
**   - 对每个 current->ptr 调 mlx_destroy_image(game->mlx, ptr)
**   - 最后 game->img_head = NULL
**
** 调用者：
**   graceful_exit()
**
** 注意：
**   这里只销毁“被 remember_image 记录的图”；
**   game->img（主画布）在 graceful_exit 里单独 destroy（因为它不在链表里）。
*/
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

/*
** 函数名：track_clean
** 作用：释放 track_malloc 系统记录的所有 malloc 内存
**
** 参数：
**   game：包含 game->track_head（t_gnode 链表）
**
** 主要逻辑：
**   - 遍历 track_head
**   - 先 free(current->ptr)（真正分配出来的内存）
**   - 再 free(current)（链表节点本身）
**   - 最后 track_head = NULL
**
** 调用者：
**   graceful_exit()
*/
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
** 函数名：graceful_exit
** 作用：统一退出函数（无论正常退出还是报错退出），负责“按顺序清理资源”
**
** 参数：
**   game：提供 mlx/win/img/链表 等资源
**   exit_code：
**     - 0 表示正常退出
**     - 非 0 表示错误退出（会打印错误信息）
**   func：错误来源函数名（__func__），exit_code!=0 时使用
**   msg：错误描述，exit_code!=0 时使用
**
** 主要逻辑（清理顺序很重要）：
**   1) destroy_images：销毁贴图等 image（链表里的）
**   2) 如果 game->img 存在：销毁主画布 image
**   3) 如果 game->win 存在：销毁窗口
**   4) 如果 game->mlx 存在：
**      - mlx_destroy_display + free(game->mlx)
**      - 置 NULL，避免悬空指针
**   5) exit_code != 0：print_error(func, msg)
**   6) track_clean：释放所有 track_malloc 记录的内存
**   7) exit(exit_code)
**
** 调用者：
**   - 任何地方需要退出时（比如解析失败、mlx 初始化失败、按 ESC）
*/
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
