/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_kind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:49 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:42:06 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：skip_spaces
** 作用：把字符串指针往后挪，跳过开头的空格/制表符
** 参数：
**   s：原字符串指针
** 返回：
**   指向第一个“非空格/非tab”的位置
** 调用者：
**   set_input_line_type()
*/
char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

/*
** 函数名：is_map_line
** 作用：粗略判断这一行像不像地图行（不是严格校验，只是“像不像”）
** 参数：
**   line：输入行（通常已经是某一行的字符串）
** 返回：
**   true：像地图行（首个非空白字符是地图允许的开头字符）
**   false：不是
** 主要逻辑：
**   1) 跳过行首空白
**   2) 全空白 => 不是地图行
**   3) 看第一个有效字符是否属于 "01NSEWCPRD"
**      - 基础：01NSEW
**      - 这里额外写了 C P R D（给 bonus 或扩展用），是否启用由别处控制
** 调用者：
**   set_input_line_type()
*/
bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line || !line[0])
		return (false);
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '\0')
		return (false);
	if (ft_strchr("01NSEWCPRD", line[i]))
		return (true);
	return (false);
}

/*
** 函数名：kind_from_word（static）
** 作用：把配置行的“第一个单词”映射成枚举类型
** 参数：
**   first_word：例如 "NO" / "SO" / "WE" / "EA" / "F" / "C"
** 返回：
**   对应的 t_line_type：NORTH/SOUTH/WEST/EAST/FLOOR/CEILING
**   不认识则返回 WRONG
** 调用者：
**   config_kind()
*/
static t_line_type	kind_from_word(char *first_word)
{
	if (first_word == NULL)
		return (WRONG);
	if (ft_strcmp(first_word, "NO") == 0)
		return (NORTH);
	if (ft_strcmp(first_word, "SO") == 0)
		return (SOUTH);
	if (ft_strcmp(first_word, "WE") == 0)
		return (WEST);
	if (ft_strcmp(first_word, "EA") == 0)
		return (EAST);
	if (ft_strcmp(first_word, "F") == 0)
		return (FLOOR);
	if (ft_strcmp(first_word, "C") == 0)
		return (CEILING);
	return (WRONG);
}

/*
** 函数名：config_kind（static）
** 作用：判断一行是否是“配置行”，如果是，返回配置类型
** 参数：
**   game：用于 ft_split 的内存跟踪/错误处理
**   s：行内容（通常已经 skip_spaces 过）
** 返回：
**   配置类型（NORTH/SOUTH/WEST/EAST/FLOOR/CEILING），否则 WRONG
** 主要逻辑：
**   1) 用空格分割成 words
**   2) 看 words[0]（第一个单词）是什么
** 注意：
**   这里只是“判定类型”，不在这里做参数数量、格式等严格校验
** 调用者：
**   set_input_line_type()
*/
static t_line_type	config_kind(t_game *game, char *s)
{
	char		**words;
	t_line_type	type;

	words = ft_split(game, s, ' ');
	if (words == NULL)
		return (WRONG);
	type = kind_from_word(words[0]);
	return (type);
}

/*
** 函数名：set_input_line_type
** 作用：给一行打标签：空行/配置行/地图行/错误行
** 参数：
**   game：用于 config_kind 里分割
**   line：原始行字符串
** 返回：
**   EMPTY：空白行
**   NORTH/SOUTH/WEST/EAST/FLOOR/CEILING：配置行
**   MAP：地图行
**   WRONG：不认识/非法
** 主要逻辑：
**   1) line 为空 => WRONG
**   2) 跳过行首空白
**   3) 只剩 '\0' => EMPTY
**   4) 如果能识别为配置关键字 => 返回配置类型
**   5) 否则如果像地图行 => MAP
**   6) 否则 => WRONG
** 调用者：
**   parse_config(), parse_utils.c 里的 find_map_start/set_map_dimensions/check_map_is_last
*/
t_line_type	set_input_line_type(t_game *game, char *line)
{
	char		*s;
	t_line_type	type;

	if (line == NULL)
		return (WRONG);
	s = skip_spaces(line);
	if (*s == '\0')
		return (EMPTY);
	type = config_kind(game, s);
	if (type != WRONG)
		return (type);
	if (is_map_line(s))
		return (MAP);
	return (WRONG);
}
