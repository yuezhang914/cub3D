/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:17 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 17:40:34 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：check_args（static）
** 作用：检查命令行参数是否符合要求：
**       1) 必须只有一个参数（地图路径）
**       2) 必须以 ".cub" 结尾
** 参数：
**   game：全局上下文，用于错误时 graceful_exit
**   argc/argv：main 传入的参数
** 返回：
**   无（出错直接 graceful_exit 退出）
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
** 函数：import_cub（static）
** 作用：读入 .cub 文件，并把内容保存到：
**       - game->entire_cubfile：整文件字符串
**       - game->cubfile_lines：按行切分后的字符串数组
** 参数：
**   game：总结构体（存放读取结果）
**   path：地图文件路径（argv[1]）
** 返回：
**   无（出错直接 graceful_exit 退出）
** 说明：
**   这里把 fd 作为局部变量使用，读完立即 close(fd)，避免在 t_game 里增加无用成员。
*/
static void	import_cub(t_game *game, char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		graceful_exit(game, 1, __func__, "Open() failed.");
	game->entire_cubfile = ft_readfile(game, fd);
	close(fd);
	if (game->entire_cubfile == NULL || *game->entire_cubfile == '\0')
		graceful_exit(game, 1, __func__, "Empty .cub-file.");
	game->cubfile_lines = splitlines(game, game->entire_cubfile);
	if (game->cubfile_lines == NULL)
		graceful_exit(game, 1, __func__, "splitlines() failed.");
}

/*
** 函数：module_parse
** 作用：解析模块总入口：
**       1) 检查参数
**       2) 读取 .cub 文件（整文本 + lines）
**       3) 解析配置（NO/SO/WE/EA/F/C）
**       4) 解析地图并提取玩家
** 参数：
**   game：总结构体
**   argc/argv：main 传入
** 返回：
**   无（出错直接 graceful_exit 退出）
*/
void	module_parse(t_game *game, int argc, char **argv)
{
	check_args(game, argc, argv);
	import_cub(game, argv[1]);
	parse_config(game);
	parse_map(game);
}
