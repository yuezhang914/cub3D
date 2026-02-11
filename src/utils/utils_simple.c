/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:35 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 17:32:52 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

// 统计字符串 str 中字符 c 出现的次数。
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

// 判断一个字符是不是数字 '0'~'9'
bool	is_dig(char c)
{
	if ((c >= '0' && c <= '9'))
		return (true);
	else
		return (false);
}

// 把 str1 跟后面传入的 n 个字符串依次比较，
// 如果相等就返回“匹配到的那个字符串指针”，否则返回 NULL
/*char	*ft_n_strcmp(char *str1, int n, ...)
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
}*/

// 把 src 的前 n 个字节复制到 dest（内存拷贝）
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

