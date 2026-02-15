/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:56 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 21:45:54 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	build_map_array(t_game *game, char **lines, int start)
{
	int	i;
	int	j;

	game->map = track_malloc(game, (game->map_h + 1) * sizeof(char *));
	i = 0;
	while (i < game->map_h)
	{
		game->map[i] = track_malloc(game, (game->map_w + 1) * sizeof(char));
		j = 0;
		while (lines[start + i][j])
		{
			game->map[i][j] = lines[start + i][j];
			j++;
		}
		while (j < game->map_w)
		{
			game->map[i][j] = ' ';
			j++;
		}
		game->map[i][j] = '\0';
		i++;
	}
	game->map[i] = NULL;
}

static void	validate_open_walls(t_game *game, int i, int j)
{
	if (i == 0 || j == 0 || i == game->map_h - 1 || j == game->map_w - 1)
		graceful_exit(game, 1, __func__, "Open wall found.");
	if (game->map[i - 1][j] == ' ' || game->map[i + 1][j] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
	if (game->map[i][j - 1] == ' ' || game->map[i][j + 1] == ' ')
		graceful_exit(game, 1, __func__, "Open wall found.");
}

static void	extract_player(t_game *game, int i, int j, bool *found)
{
	if (*found)
		graceful_exit(game, 1, __func__, "Multiple start pos.");
	*found = true;
	game->player.x = (float)j + 0.5f;
	game->player.y = (float)i + 0.5f;
	game->player.map_x = j;
	game->player.map_y = i;
	if (game->map[i][j] == 'E')
		game->player.angle = PI * 0.0f;
	else if (game->map[i][j] == 'S')
		game->player.angle = PI * 1.5f;
	else if (game->map[i][j] == 'W')
		game->player.angle = PI * 1.0f;
	else if (game->map[i][j] == 'N')
		game->player.angle = PI * 0.5f;
	game->map[i][j] = '0';
}

static void	scan_map(t_game *game)
{
	int		i;
	int		j;
	bool	found;

	found = false;
	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (ft_strchr("NESW", game->map[i][j]))
				extract_player(game, i, j, &found);
			if (game->map[i][j] == '0')
				validate_open_walls(game, i, j);
			j++;
		}
		i++;
	}
	if (!found)
		graceful_exit(game, 1, __func__, "No start pos found.");
}

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
