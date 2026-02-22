/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:21 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 03:18:57 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	count_sprites(t_game *game)
{
	int	i;
	int	j;

	if (!game || !game->map)
		return ;
	game->sprs.num = 0;
	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (ft_strchr("TBCM", game->map[i][j]))
				game->sprs.num++;
		}
	}
}

void	handle_bonus_setup(t_game *game)
{
	if (!game || !game->map)
		return ;
	count_sprites(game);
	if (game->sprs.num > 0)
	{
		game->sprs.list = track_malloc(game, sizeof(t_sprite) * game->sprs.num);
		collect_sprites(game);
	}
	init_doors(game);
}
