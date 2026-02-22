/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 17:49:30 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:47:32 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

# define ALLOWED_TILES " 01NSEWCPRDdTBCM"
# define WALKABLE_TILES "0NESWCPRDdTBCM"

#else

# define ALLOWED_TILES " 01NSEW"
# define WALKABLE_TILES "0NESW"

#endif

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
		graceful_exit(game, 1, __func__, "Multiple start position.");
	*found = true;
	game->player.x = (float)j + 0.5f;
	game->player.y = (float)i + 0.5f;
	game->player.map_x = j;
	game->player.map_y = i;
	if (game->map[i][j] == 'E')
		game->player.angle = PI * 0.0f;
	if (game->map[i][j] == 'N')
		game->player.angle = PI * 0.5f;
	if (game->map[i][j] == 'W')
		game->player.angle = PI * 1.0f;
	if (game->map[i][j] == 'S')
		game->player.angle = PI * 1.5f;
	game->map[i][j] = '0';
}

static bool	is_allowed_tile(char c)
{
	return (ft_strchr(ALLOWED_TILES, c) != NULL);
}

static bool	is_walkable_tile(char c)
{
	return (ft_strchr(WALKABLE_TILES, c) != NULL);
}

void	scan_map(t_game *game)
{
	int		i;
	int		j;
	bool	found;
	char	c;

	found = false;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			c = game->map[i][j];
			if (!is_allowed_tile(c))
				graceful_exit(game, 1, __func__, "Invalid map character.");
			if (ft_strchr("NESW", c))
				extract_player(game, i, j, &found);
			if (is_walkable_tile(c))
				validate_open_walls(game, i, j);
		}
	}
	if (!found)
		graceful_exit(game, 1, __func__, "No start position found.");
}
