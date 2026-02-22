/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:08 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:42:27 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：is_config_complete（static）
** 作用：判断“6 个必要配置”是否都已经读到了
** 必要配置：
**   4 个贴图路径：NO/SO/WE/EA（north/south/west/east）
**   2 个颜色：F（floor_color）和 C（ceiling_color）
** 参数：
**   game：存放配置结果
** 返回：
**   true：全部都有了
**   false：还缺东西
** 调用者：
**   parse_config()：遇到 MAP 行时，用它决定“能不能开始解析地图”
*/
static bool	is_config_complete(t_game *game)
{
	if (game->north.path && game->south.path && game->west.path
		&& game->east.path && game->floor_color != -1
		&& game->ceiling_color != -1)
		return (true);
	return (false);
}

/*
** 函数名：set_texture_path（static）
** 作用：解析一行贴图配置（NO/SO/WE/EA），把路径保存到对应的 game->xxx.path
** 参数：
**   game：保存路径
**   type：NORTH/SOUTH/WEST/EAST
**   line：这一整行的字符串（例如 "NO ./path/to/wall.xpm"）
** 主要逻辑：
**   1) 根据 type 决定 target 指向哪个 path 指针
**   2) 如果该贴图已经被设置过 => 报“重复定义”
**   3) ft_split 用空格拆开：必须刚好 2 个词（key + path）
**   4) path 必须以 .xpm 结尾（长度至少 5，最后 4 个字符 ".xpm"）
**   5) 把 words[1] 直接作为路径保存（注意：这里假设 ft_split 的内存被 track 追踪管理）
** 调用者：
**   parse_config()
*/
static void	set_texture_path(t_game *game, t_line_type type, char *line)
{
	char	**words;
	char	**target;
	size_t	len;

	/* 根据类型选中要写入的目标指针 */
	if (type == NORTH)
		target = &game->north.path;
	else if (type == SOUTH)
		target = &game->south.path;
	else if (type == WEST)
		target = &game->west.path;
	else
		target = &game->east.path;

	/* 防止重复定义同一个贴图 */
	if (*target != NULL)
		graceful_exit(game, 1, __func__, "Redefinition of texture path.");

	/* 拆分：期望格式 "NO path" 只有两个词 */
	words = ft_split(game, line, ' ');
	if (words[1] == NULL || words[2] != NULL)
		graceful_exit(game, 1, __func__, "Texture line arg number error.");

	/* 检查后缀 .xpm */
	len = ft_strlen(words[1]);
	if (len < 5 || ft_strcmp(".xpm", words[1] + len - 4) != 0)
		graceful_exit(game, 1, __func__, "Texture not ending with .xpm.");

	/* 保存路径（这里直接引用 words[1]） */
	*target = words[1];
}

/*
** 函数名：read_rgb_value（static）
** 作用：从字符串 s 的当前位置开始，读一个 0~255 的整数（跳过空白）
** 参数：
**   s：要解析的字符串（例如 "F 220,100,0"）
**   i：当前下标指针（读完会推进 *i）
**   out：读出的数值写到这里
** 返回：
**   false：成功读到一个合法的 0~255
**   true：失败（不是数字/超过 255）
** 主要逻辑：
**   1) 跳过空格和 tab
**   2) 如果当前不是数字 => 失败
**   3) 连续读数字，算出 val；如果 val > 255 => 失败
**   4) 再跳过空白
**   5) out = val
** 调用者：
**   set_color_config()
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
** 函数名：set_color_config（static）
** 作用：解析颜色行（F 或 C），把 RGB 变成一个 int 颜色值保存
** 参数：
**   game：保存到 game->floor_color 或 game->ceiling_color
**   type：FLOOR 或 CEILING
**   line：整行字符串，例如 "F 220,100,0"
** 主要逻辑：
**   1) 根据 type 选中目标 int 指针（floor 或 ceiling）
**   2) 防止重复定义：目标不是 -1 就报错
**   3) 跳过空白后跳过首字母（F 或 C）
**   4) 用 read_rgb_value 读 r,g,b，并严格要求中间是逗号 ','
**   5) 读完后只能剩空白+结束符 '\0'，否则格式不对
**   6) 把 RGB 打包成 0xRRGGBB： (r<<16) | (g<<8) | b
** 调用者：
**   parse_config()
*/
static void	set_color_config(t_game *game, t_line_type type, char *line)
{
	int	*i_target;
	int	i;
	int	r;
	int	g;
	int	b;

	/* 默认指向 ceiling，若是 FLOOR 再改 */
	i_target = &game->ceiling_color;
	if (type == FLOOR)
		i_target = &game->floor_color;

	/* 防止重复设置 */
	if (*i_target != -1)
		graceful_exit(game, 1, __func__, "Redefinition of color config.");

	/* i 指向首个非空白，然后 i++ 跳过 'F' 或 'C' */
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i++;

	/* 严格格式：number , number , number */
	if (read_rgb_value(line, &i, &r) || line[i++] != ',' || read_rgb_value(line,
			&i, &g) || line[i++] != ',' || read_rgb_value(line, &i, &b))
		graceful_exit(game, 1, __func__, "Bad color format.");

	/* 允许末尾空白，但不允许多余字符 */
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] != '\0')
		graceful_exit(game, 1, __func__, "Bad color format.");

	/* RGB 打包成一个 int：0xRRGGBB */
	*i_target = (r << 16) | (g << 8) | b;
}

/*
** 函数名：parse_config
** 作用：扫描 .cub 的行数组，解析“配置区”，直到遇到 MAP 行为止
** 参数：
**   game：保存解析结果（贴图路径/颜色）
** 主要逻辑：
**   逐行判断类型：
**   - EMPTY：空行，跳过
**   - FLOOR/CEILING：解析颜色
**   - NORTH/SOUTH/WEST/EAST：解析贴图路径
**   - MAP：
**       * 如果配置还没齐（is_config_complete==false）=> 报错：map 在 config 前出现
**       * 如果配置齐了 => 停止本函数（map 留给 parse_map 处理）
**   - 其他：报错（配置区出现非法行）
**   最后一直没遇到 MAP：报错 No map found
** 调用者：
**   module_parse()
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
			; /* 空行不做事 */
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
