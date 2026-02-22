/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:35 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:42:18 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：char_count
** 作用：统计字符串 str 中字符 c 出现了多少次
**
** 参数：
**   str：输入字符串（允许 NULL）
**   c：要统计的字符
**
** 返回：
**   - str==NULL：返回 -1（表示输入不合法）
**   - 否则：返回出现次数（>=0）
**
** 调用者：
**   splitlines()：统计 '\n' 的数量，用于计算行数
*/
int	char_count(char *str, char c)
{
	int	count;

	count = 0;
	if (!str)
		return (-1);
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

/*
** 函数名：is_dig
** 作用：判断一个字符是否是数字 '0'~'9'
**
** 参数：
**   c：要判断的字符
**
** 返回：
**   true：是数字
**   false：不是
**
** 调用者：
**   parse_config.c 的 read_rgb_value()：解析颜色时判断是否为数字
*/
bool	is_dig(char c)
{
	if ((c >= '0' && c <= '9'))
		return (true);
	else
		return (false);
}

/*
** 函数名：ft_n_strcmp
** 作用：把 str1 和后面给的 n 个字符串逐个比较，
**      只要有一个完全相等（ft_strcmp==0），就返回那个匹配到的字符串指针
**
** 参数：
**   str1：要对比的目标字符串
**   n：后面可变参数字符串的数量
**   ...：n 个 char* 字符串
**
** 返回：
**   - 匹配成功：返回“那个匹配的字符串指针”（注意：返回的是参数里传进来的指针）
**   - 全部不匹配：返回 NULL
**
** 主要逻辑：
**   - va_start/va_arg 逐个取出字符串 str
**   - if ft_strcmp(str, str1)==0：返回 str
**   - 最后 va_end 并返回 NULL
**
** 调用者：
**   你项目里可能用于“关键字集合匹配”（比如判断 token 是否属于某些固定字符串）
*/
char	*ft_n_strcmp(char *str1, int n, ...)
{
	va_list	strs;
	char	*str;

	va_start(strs, n);
	while (n)
	{
		str = va_arg(strs, char *);
		if (ft_strcmp(str, str1) == 0)
		{
			va_end(strs);
			return (str);
		}
		n--;
	}
	va_end(strs);
	return (NULL);
}

/*
** 函数名：ft_memcpy
** 作用：把 src 的前 n 个字节复制到 dest（不处理重叠情况）
**
** 参数：
**   dest：目标内存
**   src：源内存
**   n：复制字节数
**
** 返回：
**   - dest 或 src 为 NULL：返回 NULL
**   - 否则：返回 dest
**
** 调用者：
**   splitlines.c：复制一行字符串片段到新分配的 buffer
*/
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest2;
	unsigned char	*src2;

	dest2 = (unsigned char *)dest;
	src2 = (unsigned char *)src;
	if (!dest || !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest2[i] = src2[i];
		i++;
	}
	return (dest);
}
