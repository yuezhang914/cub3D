/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:46:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 11:46:48 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	check_map_is_last(t_game *game, char **lines, int start)
{
	int	i;

	i = start;
	while (lines[i] && set_input_line_type(game, lines[i]) == MAP)
		i++;
	while (lines[i] && set_input_line_type(game, lines[i]) == EMPTY)
		i++;
	if (lines[i] != NULL)
		graceful_exit(game, 1, __func__, "Non-empty line after map.");
}

void	set_map_dimensions(t_game *game, char **lines, int start)
{
	int	i;
	int	w;

	i = start;
	while (lines[i] && set_input_line_type(game, lines[i]) == MAP)
	{
		w = (int)ft_strlen(lines[i]);
		if (w > game->map_w)
			game->map_w = w;
		game->map_h++;
		i++;
	}
}

int	find_map_start(t_game *game, char **lines)
{
	int	start;

	start = 0;
	while (lines[start] && set_input_line_type(game, lines[start]) != MAP)
		start++;
	if (lines[start] == NULL)
		graceful_exit(game, 1, __func__, "No map line found.");
	return (start);
}
