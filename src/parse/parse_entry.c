/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:17 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:41:46 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：check_args（static）
** 作用：检查命令行参数是否正确：必须只给 1 个参数，并且以 .cub 结尾
** 参数：
**   game：全局游戏对象（用于 graceful_exit 统一退出）
**   argc/argv：main 的参数
** 主要逻辑：
**   1) argc 必须为 2（程序名 + cub 文件路径）
**   2) argv[1] 长度至少 5（例如 "a.cub"）
**   3) 最后 4 个字符必须是 ".cub"
** 调用者：
**   module_parse()
*/
static void	check_args(t_game *game, int argc, char **argv)
{
	size_t	len;

	if (argc != 2)
		graceful_exit(game, 1, __func__, "Wrong argument number.");
	len = ft_strlen(argv[1]);
	if (len < 5)
		graceful_exit(game, 1, __func__, "Argument should end with '.cub'.");
	if (ft_strcmp(".cub", argv[1] + len - 4) != 0)
		graceful_exit(game, 1, __func__, "Argument should end with '.cub'.");
}

/*
** 函数名：import_cub（static）
** 作用：把 .cub 文件完整读进内存，并按行切成字符串数组
** 参数：
**   game：保存读取到的内容（entire_cubfile / cubfile_lines）
**   path：.cub 文件路径
** 主要逻辑：
**   1) open 打开文件
**   2) ft_readfile 一次性读完（返回整个字符串）
**   3) splitlines 把整个字符串按 '\n' 分割成 char**（每一行一个字符串）
**   4) 空文件、读取失败、分割失败都要报错退出
** 调用者：
**   module_parse()
*/
static void	import_cub(t_game *game, char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		graceful_exit(game, 1, __func__, "Open() failed.");
	game->entire_cubfile = ft_readfile(game, fd);
	close(fd);
	/* 读出来为空或者内容为 '\0'，都视为“空的 cub 文件” */
	if (game->entire_cubfile == NULL || *game->entire_cubfile == '\0')
		graceful_exit(game, 1, __func__, "Empty .cub-file.");
	game->cubfile_lines = splitlines(game, game->entire_cubfile);
	if (game->cubfile_lines == NULL)
		graceful_exit(game, 1, __func__, "splitlines() failed.");
}

/*
** 函数名：module_parse
** 作用：解析模块入口（通常由 main 调用）
** 参数：
**   game：全局游戏对象（保存解析结果）
**   argc/argv：命令行参数
** 主要逻辑（顺序很关键）：
**   1) check_args：参数合法性（.cub）
**   2) import_cub：读文件、拆行
**   3) parse_config：解析贴图路径、地板/天花板颜色，直到遇到 map
**   4) parse_map：找到 map 起点、建立规则的二维 map 数组、扫描校验、提取出生点等
** 调用者：
**   main()
*/
void	module_parse(t_game *game, int argc, char **argv)
{
	check_args(game, argc, argv);
	import_cub(game, argv[1]);
	parse_config(game);
	parse_map(game);
}
