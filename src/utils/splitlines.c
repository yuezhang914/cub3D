/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitlines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:52 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 17:49:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

/*
** 函数名：sfill_array（static）
** 作用：把 text 里的每一行复制到 result[i] 里（真正干活的填充函数）。
**
** 参数：
**   game：用于 track_malloc
**   pos：当前扫描的位置（一般一开始就是 text）
**   line_count：总行数（提前算好的）
**   result：splitlines 分配好的 char**，要被填满
**
** 返回：
**   目前实现永远返回 0（成功），如果你以后想增强，可以在分配失败时返回 1。
**
** 用在哪里：
**   只在本文件 splitlines 内部使用（static 表示外部看不到它）。
*/
static int	sfill_array(t_game *game, char *pos, int line_count, char **result)
{
	size_t	len;
	int		i;

	i = 0;
	while (i < line_count - 1)
	{
		len = ft_strchr(pos, '\n') - pos;
		result[i] = track_malloc(game, (len + 1) * sizeof(char));
		ft_memcpy(result[i], pos, len);
		result[i][len] = '\0';
		pos = ft_strchr(pos, '\n') + 1;
		i++;
	}
	len = ft_strchr(pos, '\0') - pos;
	result[i] = track_malloc(game, (len + 1) * sizeof(char));
	ft_memcpy(result[i], pos, len);
	result[i][len] = '\0';
	result[line_count] = NULL;
	return (0);
}

/*
** 函数名：splitlines
** 作用：把一个大字符串 text 按 '\n' 切成二维数组（每个元素是一行）。
**      和 ft_split(text, '\n') 类似，但它的特点是：会保留空行。
**
** 参数：
**   game：用于 track_malloc 分配二维数组和每一行字符串
**   text：要切分的原始大字符串（例如整个 .cub 文件）
**
** 返回：
**   成功：char**，形如：
**       result[0] = 第1行字符串
**       result[1] = 第2行字符串
**       ...
**       result[last] = NULL（结束标记）
**   失败：NULL
**
** 用在哪里：
**   解析 .cub 时：
**     entire_cubfile = ft_readfile(...)
**     cubfile_lines = splitlines(game, entire_cubfile)
**
** 为什么要“保留空行”：
**   subject 允许配置区出现空行，用 splitlines 能让 parser 更容易处理这些空行。
*/
char	**splitlines(t_game *game, char *text)
{
	int		line_count;
	char	**result;

	line_count = char_count(text, '\n') + 1;
	if (line_count == 0)
		return (NULL);
	result = track_malloc(game, (line_count + 1) * sizeof(void *));
	if (result == NULL)
		return (NULL);
	if (sfill_array(game, text, line_count, result) == 1)
		return (NULL);
	return (result);
}
