/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:14:14 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:42:04 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：track_add_node
** 作用：把一块已经 malloc 得到的 ptr，记录到 game->track_head 链表中
**
** 参数：
**   game：包含 track_head
**   ptr：刚 malloc 成功的内存地址
**
** 返回：
**   成功：新创建的链表节点指针
**   失败：NULL（节点分配失败）
**
** 主要逻辑：
**   - malloc 一个 t_gnode
**   - new_node->ptr = ptr
**   - 头插到 game->track_head
**
** 调用者：
**   track_malloc()
*/
t_gnode	*track_add_node(t_game *game, void *ptr)
{
	t_gnode	*new_node;

	new_node = (t_gnode *)malloc(sizeof(t_gnode));
	if (!new_node)
		return (NULL);
	new_node->ptr = ptr;
	new_node->next = game->track_head;
	game->track_head = new_node;
	return (new_node);
}

/*
** 函数名：track_malloc
** 作用：替代 malloc：分配内存并自动“登记”，以后 graceful_exit 里可统一释放
**
** 参数：
**   game：用于记录链表
**   size：要分配的字节数
**
** 返回：
**   成功：分配到的 ptr
**   失败：NULL
**
** 主要逻辑：
**   1) ptr = malloc(size)
**   2) track_add_node(game, ptr) 登记
**      - 如果登记失败：free(ptr) 并返回 NULL
**
** 调用者：
**   几乎所有需要 malloc 的地方：ft_split、ft_strdup、splitlines、ft_readfile 等
*/
void	*track_malloc(t_game *game, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	if (track_add_node(game, ptr) == NULL)
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}
