/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprites_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>                     +#+  +:+       +#*/
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:35:12 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/21 18:35:14 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_static_sprites(t_game *game)
{
	game->config[SPR_TREE].frame_count = 1;
	game->config[SPR_TREE].is_animated = false;
	game->config[SPR_TREE].is_directional = false;
	game->config[SPR_TREE].v_div = 1.0f;
	game->config[SPR_TREE].h_div = 1.0f;
	game->config[SPR_TREE].v_move = 0.0f;
	game->config[SPR_BARREL].frame_count = 1;
	game->config[SPR_BARREL].is_animated = false;
	game->config[SPR_BARREL].is_directional = false;
	game->config[SPR_BARREL].v_div = 2.0f;
	game->config[SPR_BARREL].h_div = 2.0f;
	game->config[SPR_BARREL].v_move = 300.0f;
}

static void	init_dynamic_sprites(t_game *game)
{
	game->config[SPR_TORCH].frame_count = 4;
	game->config[SPR_TORCH].is_animated = true;
	game->config[SPR_TORCH].is_directional = false;
	game->config[SPR_TORCH].v_div = 1.0f;
	game->config[SPR_TORCH].h_div = 1.0f;
	game->config[SPR_TORCH].v_move = -320.0f;
	game->config[SPR_MONSTER].frame_count = 8;
	game->config[SPR_MONSTER].is_animated = false;
	game->config[SPR_MONSTER].is_directional = true;
	game->config[SPR_MONSTER].v_div = 2.0f;
	game->config[SPR_MONSTER].h_div = 2.0f;
	game->config[SPR_MONSTER].v_move = 300.0f;
}

void	init_sprite_texture(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	init_static_sprites(game);
	init_dynamic_sprites(game);
	load_all_sprite_resources(game);
}
