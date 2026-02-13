/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:17 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 17:49:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"
#include "func.h"
/*
** 作用：检查命令行参数是否正确
** 参数：
**   game：全局上下文，用于报错时统一退出
**   argc/argv：main 传入
** 返回：无（错误就退出）
*/
static void	check_args(t_game *game, int argc, char **argv)
{
	size_t	len;

	/* 必须只有 1 个参数：地图文件路径 */
	if (argc != 2)
		graceful_exit(game, 1, __func__, "Wrong argument number.");

	/* 必须以 .cub 结尾 */
	len = ft_strlen(argv[1]);
	if (len < 5)
		graceful_exit(game, 1, __func__, "Argument should end with '.cub'.");
	if (ft_strcmp(".cub", argv[1] + len - 4) != 0)
		graceful_exit(game, 1, __func__, "Argument should end with '.cub'.");
}

/*
** 作用：读入 .cub 文件，得到整文件字符串 + 按行切分后的 lines
** 参数：
**   game：保存 fd / 整文件内容 / lines
**   path：argv[1]
** 返回：无（错误就退出）
*/
static void	import_cub(t_game *game, char *path)
{
	game->cubfile_fd = open(path, O_RDONLY);
	if (game->cubfile_fd < 0)
		graceful_exit(game, 1, __func__, "Open() failed.");

	game->entire_cubfile = ft_readfile(game, game->cubfile_fd);
	if (game->entire_cubfile == NULL || *game->entire_cubfile == '\0')
		graceful_exit(game, 1, __func__, "Empty .cub-file.");

	game->cubfile_lines = splitlines(game, game->entire_cubfile);
	if (game->cubfile_lines == NULL)
		graceful_exit(game, 1, __func__, "ft_split lines failed.");
}

/*
** 作用：parse 总入口：参数检查 + 读文件 + 解析配置 + 解析地图
** 参数：
**   game：全局上下文
**   argc/argv：main 传入
*/
void	module_parse(t_game *game, int argc, char **argv)
{
	check_args(game, argc, argv);
	import_cub(game, argv[1]);

	/* 先配置，后地图（地图必须是最后一段） */
	parse_config(game);
	parse_map(game);
}
