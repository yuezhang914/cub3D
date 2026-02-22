/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:56 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:44:56 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	build_map_array(t_game *game, char **lines, int start)
{
	int	i;
	int	j;

	game->map = track_malloc(game, (game->map_h + 1) * sizeof(char *));
	i = -1;
	while (++i < game->map_h)
	{
		game->map[i] = track_malloc(game, (game->map_w + 1) * sizeof(char));
		j = 0;
		while (lines[start + i][j])
		{
			game->map[i][j] = lines[start + i][j];
			if (lines[start + i][j] == '\t')
				game->map[i][j] = ' ';
			j++;
		}
		while (j < game->map_w)
			game->map[i][j++] = ' ';
		game->map[i][j] = '\0';
	}
	game->map[i] = NULL;
}

#ifdef BONUS

void	parse_map(t_game *game)
{
	int		start;
	char	**lines;

	lines = game->cubfile_lines;
	start = find_map_start(game, lines);
	check_map_is_last(game, lines, start);
	set_map_dimensions(game, lines, start);
	build_map_array(game, lines, start);
	scan_map(game);
	handle_bonus_setup(game);
}

#else

void	parse_map(t_game *game)
{
	int		start;
	char	**lines;

	lines = game->cubfile_lines;
	start = find_map_start(game, lines);
	check_map_is_last(game, lines, start);
	set_map_dimensions(game, lines, start);
	build_map_array(game, lines, start);
	scan_map(game);
}
#endif
