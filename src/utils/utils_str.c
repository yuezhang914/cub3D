/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:07:51 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:42:31 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：ft_strlen
** 作用：返回字符串长度（不包含 '\0'）
**
** 参数：
**   s：字符串指针（允许 NULL）
**
** 返回：
**   - s==NULL：返回 0（你这里选择“NULL 长度为 0”的策略）
**   - 否则：返回字符数
**
** 调用者：
**   - put_str/print_error
**   - ft_strdup/ft_strjoin
**   - 各种需要长度的地方
*/
size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
** 函数名：ft_strcmp
** 作用：比较两个字符串（类似标准 strcmp）
**
** 参数：
**   str1/str2：要比较的两个字符串（注意：你这里没做 NULL 保护）
**
** 返回：
**   - 第一个不同字符的差值 (unsigned char)str1 - (unsigned char)str2（你这里直接 char 差）
**   - 如果完全相同：返回 0
**
** 主要逻辑：
**   - 一直走到 str1 结束
**   - 如果中途某个字符不等，返回差值
**   - 最后返回 '\0' 与对应字符的差（处理前缀情况）
**
** 调用者：
**   - ft_n_strcmp
**   - parse 模块：判断 "NO"/"SO"/"F"/"C" 等关键字
*/
int	ft_strcmp(char *str1, char *str2)
{
	while (*str1)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

/*
** 函数名：ft_strchr
** 作用：在字符串 s 中查找第一次出现字符 c 的位置
**
** 参数：
**   s：字符串
**   c：要找的字符（int，内部当 unsigned char 用）
**
** 返回：
**   - 找到：返回指向该字符的指针
**   - 找不到：返回 NULL
**   - 如果 c 是 '\0'：返回指向字符串结尾 '\0' 的指针
**
** 调用者：
**   - splitlines.c：找 '\n' 或 '\0'
**   - parse/其它模块：判断某字符是否在集合里（你也有 ft_strchr("01NSEW", ch) 这种用法）
*/
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/*
** 函数名：ft_strdup
** 作用：复制字符串 s 到一块新内存（用 track_malloc 管理）
**
** 参数：
**   game：用于 track_malloc（统一释放）
**   s：源字符串（允许 NULL）
**
** 返回：
**   成功：新字符串指针
**   失败：NULL
**
** 调用者：
**   项目中需要保存某个字符串副本时（避免引用临时 buffer）
*/
char	*ft_strdup(t_game *game, const char *s)
{
	char	*str;
	int		i;

	if (!s)
		return (NULL);
	str = track_malloc(game, (ft_strlen(s) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
** 函数名：ft_strjoin
** 作用：拼接两个字符串 s1+s2，返回新字符串（用 track_malloc 管理）
**
** 参数：
**   game：用于 track_malloc
**   s1/s2：要拼接的字符串（注意：你这里假设 s1/s2 都非 NULL）
**
** 返回：
**   新字符串指针，失败返回 NULL
**
** 主要逻辑：
**   1) size_needed = len(s1)+len(s2)+1
**   2) 分配 s3
**   3) 依次复制 s1、再复制 s2
**   4) 补 '\0'
**   5) 因为 s3 指针在复制中被不断 ++，最后要减回起点：
**      s3 -= (size_needed - 1)
**
** 调用者：
**   ft_readfile：不断把 buf 拼到 str 后面
*/
char	*ft_strjoin(t_game *game, char const *s1, char const *s2)
{
	size_t	size_needed;
	char	*s3;

	size_needed = ft_strlen(s1) + ft_strlen(s2) + 1;
	s3 = track_malloc(game, size_needed * sizeof(char));
	if (!s3)
		return (NULL);
	while (*s1)
		*s3++ = *s1++;
	while (*s2)
		*s3++ = *s2++;
	*s3 = '\0';
	s3 -= (size_needed - 1);
	return (s3);
}
