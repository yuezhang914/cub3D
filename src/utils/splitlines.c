/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitlines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:11:52 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:41:48 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：sfill_array（static）
** 作用：把 text 按 '\n' 切开，逐行复制到 result 数组里
**
** 参数：
**   game：用于 track_malloc
**   pos：当前扫描位置（一开始通常是 text）
**   line_count：总行数（= '\n' 数量 + 1）
**   result：输出的 char**（外部已分配好指针数组）
**
** 返回：
**   0：成功（这份代码里始终返回 0）
**
** 主要逻辑：
**   - 前 line_count-1 行：每次找下一个 '\n'
**     len = strchr(pos,'\n') - pos
**     分配 len+1，memcpy，补 '\0'
**     pos 跳到 '\n'+1 继续
**   - 最后一行：找 '\0' 作为结尾
**
** 调用者：
**   splitlines()
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
** 作用：把一整段文本 text 按行拆成 char**（每个元素一行，不含 '\n'）
**
** 参数：
**   game：用于 track_malloc
**   text：完整文本（必须是 '\0' 结尾的 C 字符串）
**
** 返回：
**   成功：char**，最后一个元素为 NULL
**   失败：NULL
**
** 主要逻辑：
**   1) line_count = char_count(text, '\n') + 1
**      - 如果 text 里有 N 个 '\n'，就有 N+1 行
**   2) 分配 result 指针数组：line_count + 1（最后 NULL）
**   3) 调 sfill_array 把每行复制进去
**
** 调用者：
**   parse_entry.c import_cub：把整个 .cub 文件按行变成 cubfile_lines
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
