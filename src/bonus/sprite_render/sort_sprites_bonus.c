/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:33:51 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:33:56 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_sprite_distances(t_game *game)
{
	int		i;
	float	dx;
	float	dy;

	i = 0;
	while (i < game->sprs.num)
	{
		dx = game->player.x - game->sprs.list[i].x;
		dy = game->player.y - game->sprs.list[i].y;
		game->sprs.list[i].dist = (dx * dx) + (dy * dy);
		i++;
	}
}

#ifdef BONUS

void	sort_sprites(t_game *game)
{
	int			i;
	int			j;
	t_sprite	temp;

	if (game->sprs.num < 2)
		return ;
	update_sprite_distances(game);
	i = 0;
	while (i < game->sprs.num - 1)
	{
		j = 0;
		while (j < game->sprs.num - i - 1)
		{
			if (game->sprs.list[j].dist < game->sprs.list[j + 1].dist)
			{
				temp = game->sprs.list[j];
				game->sprs.list[j] = game->sprs.list[j + 1];
				game->sprs.list[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
#endif
