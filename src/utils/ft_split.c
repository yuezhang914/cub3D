/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:08:07 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:41:32 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：index_substr（static）
** 作用：返回第 index 个“子串”的起始指针（以 dlm 为分隔符，连续 dlm 视为一个分隔区）
**
** 参数：
**   str：原字符串
**   index：第几个子串（从 0 开始）
**   dlm：分隔符字符
**
** 返回：
**   - 找到：指向第 index 个子串起点的指针
**   - 找不到：NULL（说明 index 超出子串数量）
**
** 主要逻辑（关键点：跳过连续分隔符）：
**   char_sctn 用来判断“我是否处在一个子串里”
**   - 从头扫：
**     * 从“子串结束并遇到分隔符”的位置，index--（表示跨过了一个子串）
**   - 扫到目标 index 后：
**     * 再跳过若干个 dlm，定位到子串起点
**
** 调用者：
**   substr_len()
**   substr_cpy()
**   ft_split() 用它来计数子串数量
*/
static const char	*index_substr(const char *str, int index, char dlm)
{
	int	char_sctn;

	char_sctn = 0;
	while (*str && index > 0)
	{
		if (char_sctn == 1 && *str == dlm)
			index--;
		if (*str != dlm)
			char_sctn = 1;
		if (*str == dlm)
			char_sctn = 0;
		str++;
	}
	while (*str && *str == dlm)
		str++;
	if (!*str)
		return (NULL);
	return (str);
}

/*
** 函数名：substr_len（static）
** 作用：计算第 i 个子串的长度（不包含分隔符 dlm）
**
** 参数：
**   str：原字符串
**   dlm：分隔符
**   i：第 i 个子串（从 0 开始）
**
** 返回：
**   该子串长度
**
** 调用者：
**   ft_split()：分配每个子串需要的空间
*/
static int	substr_len(char const *str, char dlm, int i)
{
	int	cnt;

	str = index_substr(str, i, dlm);
	cnt = 0;
	while (*str != dlm && *str)
	{
		str++;
		cnt++;
	}
	return (cnt);
}

/*
** 函数名：substr_cpy（static）
** 作用：把第 i 个子串复制到 sub，并补 '\0'
**
** 参数：
**   str：原字符串
**   dlm：分隔符
**   i：第 i 个子串
**   sub：目标缓冲区（已分配好足够长度）
**
** 调用者：
**   ft_split()
*/
static void	substr_cpy(char const *str, char dlm, int i, char *sub)
{
	str = index_substr(str, i, dlm);
	while (*str != dlm && *str)
	{
		*sub++ = *str++;
	}
	*sub = '\0';
}

/*
** 函数名：ft_split
** 作用：把字符串 s 按分隔符 c 切成若干个子串，返回 char**（最后一个元素为 NULL）
**
** 参数：
**   game：用于 track_malloc（统一内存管理）
**   s：要分割的字符串
**   c：分隔符
**
** 返回：
**   成功：char**（每个子串都是新分配的），末尾 ptr[sub_cnt]=NULL
**   失败：NULL
**
** 主要逻辑：
**   1) sub_cnt 计数：不断调用 index_substr(s, sub_cnt, c)，直到返回 NULL
**   2) 分配指针数组 ptr（sub_cnt + 1）
**   3) for i=0..sub_cnt-1：
**      - 分配 ptr[i] 的字符数组（substr_len + 1）
**      - substr_cpy 复制子串内容
**   4) ptr[sub_cnt]=NULL
**
** 调用者：
**   - parse_line_kind.c / parse_config.c 等：分割配置行 "NO path"
*/
char	**ft_split(t_game *game, char const *s, char c)
{
	int		sub_cnt;
	char	**ptr;
	int		i;

	sub_cnt = 0;
	while (index_substr(s, sub_cnt, c))
		sub_cnt++;
	ptr = track_malloc(game, (sub_cnt + 1) * sizeof(void *));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < sub_cnt)
	{
		ptr[i] = track_malloc(game, (substr_len(s, c, i) + 1) * sizeof(char));
		if (ptr[i] == NULL)
			return (NULL);
		substr_cpy(s, c, i, ptr[i]);
		i++;
	}
	ptr[i] = NULL;
	return (ptr);
}
