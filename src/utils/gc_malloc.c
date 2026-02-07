/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:14:14 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 17:14:29 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/*
** 函数名：track_add_node
** 作用：把一块“已经 malloc 出来的内存指针 ptr”登记到 game->track_head 链表里，
**      方便程序结束时统一 free（这不是自动垃圾回收，只是“指针登记册”）。
**
** 参数：
**   game：全局上下文，里面保存登记链表头 game->track_head
**   ptr ：刚 malloc 得到的指针（要登记）
**
** 返回：
**   成功：新创建的链表节点（t_gnode*）
**   失败：NULL（表示登记失败）
**
** 用在哪里：
**   只被 track_malloc 调用。你们项目里所有“想自动统一释放的 malloc”都走 track_malloc。
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
** 作用：对 malloc 做一层“登记包装”：
**      1) malloc(size)
**      2) 把返回指针加入 game->track_head 链表
**      这样退出时遍历 track_head，就能一次性 free 所有登记过的内存。
**
** 参数：
**   game：用来保存登记链表
**   size：要分配的字节数
**
** 返回：
**   成功：新分配的内存指针
**   失败：NULL（malloc 或登记失败）
**
** 用在哪里：
**   - 读文件(ft_readfile)创建字符串
**   - splitlines/ft_split 创建二维数组与子串
**   - parse 创建 map、保存路径、各种临时结构
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
