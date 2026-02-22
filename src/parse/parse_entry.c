/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:17 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:42:34 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	module_parse(t_game *game, int argc, char **argv)
{
	check_args(game, argc, argv);
	import_cub(game, argv[1]);
	parse_config(game);
	parse_map(game);
}
