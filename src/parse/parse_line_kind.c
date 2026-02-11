/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_kind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:49 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 11:49:12 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"
#include "func.h"

/*
** 函数：skip_spaces（static）
** 作用：跳过行首空格/Tab，返回第一个非空白字符的位置
*/
char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

/*
** 函数：is_map_line（static）
** 作用：判断一行是否是地图行
** 规则：只允许包含 " 10NSWE"，且必须至少出现一个 "10NSWE"（不能全是空格）
*/
static bool	is_map_line(char *str)
{
	bool	has_tile;

	if (str == NULL)
		return (false);
	has_tile = false;
	while (*str)
	{
		if (ft_strchr(" 10NSWE", *str) == NULL)
			return (false);
		if (ft_strchr("10NSWE", *str) != NULL)
			has_tile = true;
		str++;
	}
	return (has_tile);
}

/*
** 函数：kind_from_word（static）
** 作用：把第一个单词（NO/SO/WE/EA/F/C）翻译成对应的 t_line_type
** 返回：匹配则返回类型；不匹配返回 WRONG
*/
static t_line_type	kind_from_word(char *w0)
{
	if (w0 == NULL)
		return (WRONG);
	if (ft_strcmp(w0, "NO") == 0)
		return (NORTH);
	if (ft_strcmp(w0, "SO") == 0)
		return (SOUTH);
	if (ft_strcmp(w0, "WE") == 0)
		return (WEST);
	if (ft_strcmp(w0, "EA") == 0)
		return (EAST);
	if (ft_strcmp(w0, "F") == 0)
		return (FLOOR);
	if (ft_strcmp(w0, "C") == 0)
		return (CEILING);
	return (WRONG);
}

/*
** 函数：config_kind（static）
** 作用：尝试把一行当成配置行解析：切第一个单词并映射成类型
** 返回：是配置项则返回对应类型；否则返回 WRONG
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
** 函数：set_input_line_type
** 作用：把一行分类成：EMPTY / 配置项 / MAP / WRONG
** 注意：先跳过行首空白；全空白行算 EMPTY；全空格行不会被误判成 MAP
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
