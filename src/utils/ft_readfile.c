/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:12:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:41:17 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：ft_bzero
** 作用：把一段内存清 0（等价于 memset(s, 0, n)）
** 参数：
**   s：内存起始地址
**   n：要清零的字节数（size_t）
** 主要逻辑：
**   - 把 s 转成 unsigned char*，逐字节置 0
** 调用者：
**   ft_readfile()：每次读之前清空 buf
**   也可能被别的模块当通用工具使用
*/
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
** 作用：把 fd 指向的文件“全部读完”，拼成一个字符串返回
**
** 参数：
**   game：用于 track_malloc / ft_strjoin（所有内存都挂到 track_head，方便统一释放）
**   fd：已打开的文件描述符
**
** 返回：
**   成功：指向完整文件内容的字符串（以 '\0' 结尾）
**   失败：NULL（比如 read 返回 -1 或内存分配失败）
**
** 主要逻辑（循环 read + 拼接）：
**   1) 先分配一个长度 1 的空串 str，并置 '\0'
**   2) while(1):
**      - 清空 buf
**      - read(fd, buf, BUFFER_SIZE-1)（留 1 字节给 '\0'）
**      - read==-1：失败返回 NULL
**      - read==0：EOF，break
**      - 用 ft_strjoin(game, str, buf) 拼接到新串
**        注意：这里 old str 是 track_malloc 分配的，不手动 free，
**              最终由 track_clean 统一释放
**
** 调用者：
**   parse_entry.c 里 import_cub()（读整个 .cub 文件）
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
