/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_sprites_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:41 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:35:44 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	fill_sprite_data(t_game *game, int i, int j, int *count)
{
	char	c;

	c = game->map[i][j];
	game->sprs.list[*count].x = j + 0.5f;
	game->sprs.list[*count].y = i + 0.5f;
	if (c == 'T')
		game->sprs.list[*count].type = SPR_TREE;
	else if (c == 'B')
		game->sprs.list[*count].type = SPR_BARREL;
	else if (c == 'C')
		game->sprs.list[*count].type = SPR_TORCH;
	else if (c == 'M')
		game->sprs.list[*count].type = SPR_MONSTER;
	game->map[i][j] = '0';
	(*count)++;
}

void	collect_sprites(t_game *game)
{
	int	i;
	int	j;
	int	count;

	count_sprites(game);
	if (game->sprs.num == 0)
		return ;
	game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
	if (!game->sprs.list)
		return ;
	ft_bzero(game->sprs.list, sizeof(t_sprite) * game->sprs.num);
	count = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
			if (ft_strchr("TBCM", game->map[i][j]))
				fill_sprite_data(game, i, j, &count);
	}
}
