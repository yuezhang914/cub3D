/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:08 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 17:40:28 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "func.h"

/*
**  检查game 的配置是否完备， 包括是否有四面墙的贴图路径，天花板和地板是否分配了颜色。
*/
static bool	is_config_complete(t_game *game)
{
	if (game->north.path && game->south.path && game->west.path
		&& game->east.path && game->floor_color != -1 && game->ceiling_color !=
		-1)
		return (true);
	return (false);
}

/*
** 作用：从line的内容解析四面墙的贴图路径
** 规则：必须两段：标识 + 路径；路径必须以 .xpm 结尾
*/

static void	set_texture_path(t_game *game, t_line_type type, char *line)
{
	char	**words;
	char	**target;
	size_t	len;

	if (type == NORTH)
		target = &game->north.path;
	else if (type == SOUTH)
		target = &game->south.path;
	else if (type == WEST)
		target = &game->west.path;
	else
		target = &game->east.path;
	if (*target != NULL)
		graceful_exit(game, 1, __func__, "Redefinition of texture path.");
	words = ft_split(game, line, ' ');
	// 	它在检查：这一行的参数数量必须刚好是 2 段（标识符 + 路径）。
	// 情况 A：缺路径（只有 1 段）情况 B：多了多余参数（超过 2 段）
	if (words[1] == NULL || words[2] != NULL)
		graceful_exit(game, 1, __func__, "Texture line arg number error.");
	len = ft_strlen(words[1]);
	// .xpm 本身就 4 个字符，还得至少有 1 个字符在前面
	// words[1] + len - 4 是取“最后 4 个字符”
	if (len < 5 || ft_strcmp(".xpm", words[1] + len - 4) != 0)
		graceful_exit(game, 1, __func__, "Texture not ending with .xpm.");
	*target = words[1];
}

/*
** 作用：读取一个 0~255 的整数（给颜色解析用）， 相当于对于颜色的atoi
** 返回：true 表示失败，false 表示成功
*/
static bool	read_rgb_value(char *s, int *i, int *out)
{
	int	val;

	val = 0;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!is_dig(s[*i]))
		return (true);
	while (is_dig(s[*i]))
	{
		val = val * 10 + (s[*i] - '0');
		if (val > 255)
			return (true);
		(*i)++;
	}
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	*out = val;
	return (false);
}

/*
** 作用：解析 F/C 颜色行：F 220,100,0（逗号两侧允许空格）
** 参数：game(总结构体), type(FLOOR/CEILING), line(当前行)
** 用在哪：parse_config 处理 F/C 配置项
*/
static void	set_color_config(t_game *game, t_line_type type, char *line)
{
	int	*i_target;
	int	i;
	int	r;
	int	g;
	int	b;

	i_target = &game->ceiling_color;
	if (type == FLOOR)
		i_target = &game->floor_color;
	if (*i_target != -1)
		graceful_exit(game, 1, __func__, "Redefinition of color config.");
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i++;
	if (read_rgb_value(line, &i, &r) || line[i++] != ',' || read_rgb_value(line,
			&i, &g) || line[i++] != ',' || read_rgb_value(line, &i, &b))
		graceful_exit(game, 1, __func__, "Bad color format.");
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] != '\0')
		graceful_exit(game, 1, __func__, "Bad color format.");
	*i_target = (r << 16) | (g << 8) | b;
}

/*
** 作用：解析配置区：直到遇到地图行（地图必须最后）
*/
void	parse_config(t_game *game)
{
	int			i;
	t_line_type	type;
	char		**lines;

	lines = game->cubfile_lines;
	i = 0;
	while (lines[i])
	{
		type = set_input_line_type(game, lines[i]);
		if (type == EMPTY)
			; /* 空行允许出现 */
		else if (type == FLOOR || type == CEILING)
			set_color_config(game, type, lines[i]);
		else if (type == NORTH || type == SOUTH || type == WEST || type == EAST)
			set_texture_path(game, type, lines[i]);
		else if (type == MAP && !is_config_complete(game))
			graceful_exit(game, 1, __func__, "Map before config complete.");
		else if (type == MAP)
			return ;
		else
			graceful_exit(game, 1, __func__, "Invalid line in config.");
		i++;
	}
	graceful_exit(game, 1, __func__, "No map found.");
}
