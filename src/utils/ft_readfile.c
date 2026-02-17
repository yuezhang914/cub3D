/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:12:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// 把一段内存全部置 0（清空缓冲区）
void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

/*
** 函数名：ft_readfile
** 作用：把 fd 指向的文件内容全部读出来，拼成一个大字符串返回。
**
** 参数：
**   game：用于 track_malloc + ft_strjoin（你这套工具函数都依赖 game）
**   fd  ：已经 open 并可 read 的文件描述符
**
** 返回：
**   成功：返回 “整个文件内容”的字符串（以 '\0' 结尾）
**   失败：NULL（read 出错 / 内存分配失败）
**
** 用在哪里（典型调用链）：
**   parse入口：
**     open(.cub) -> ft_readfile(game, fd) 得到 entire_cubfile
**     -> splitlines(game, entire_cubfile) 得到按行数组
**     -> 解析 NO/SO/WE/EA/F/C + map
**
** 说明（初中生版）：
**   read 每次只能读一小块，所以这里用循环一直读，读到 0 表示文件结束。
*/
char	*ft_readfile(t_game *game, int fd)
{
	char	*str;
	char	buf[BUFFER_SIZE];
	int		read_rt;

	str = track_malloc(game, sizeof(char));
	if (str == NULL)
		return (NULL);
	*str = '\0';
	while (1)
	{
		ft_bzero(buf, BUFFER_SIZE);
		read_rt = read(fd, buf, BUFFER_SIZE - 1);
		if (read_rt == -1)
			return (NULL);
		if (read_rt == 0)
			break ;
		str = ft_strjoin(game, str, buf);
		if (str == NULL)
			return (NULL);
	}
	return (str);
}
