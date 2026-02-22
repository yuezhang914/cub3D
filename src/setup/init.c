/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 20:50:12 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:52:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

static void	init_sprite_params(t_game *game)
{
	int	i;

	game->sprs.num = 0;
	game->sprs.list = NULL;
	ft_bzero(game->config, sizeof(t_sprite_config) * SPR_COUNT);
	i = -1;
	while (++i < SPR_COUNT)
	{
		game->config[i].h_div = 1.0f;
		game->config[i].v_div = 1.0f;
		game->config[i].v_move = 0.0f;
	}
	game->config[SPR_BARREL].v_move = 200.0f;
	game->config[SPR_TORCH].v_move = -400.0f;
}
#endif

#ifdef BONUS

void	init_game(t_game *game)
{
	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
	game->player.angle = 0.0f;
	game->player.mouse_enabled = 1;
	game->player.mouse_sens = MOUSE_SENS;
	init_sprite_params(game);
	game->pix_per_unit = 8.0f;
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
}
#else

void	init_game(t_game *game)
{
	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	game->player.move_speed = 0.05f;
	game->player.rot_speed = 0.04f;
	game->player.angle = 0.0f;
	game->player.mouse_enabled = 0;
	game->player.mouse_sens = 0.0f;
	game->pix_per_unit = 8.0f;
	game->focal_length = 2.0f * tan((FOV * (float)PI / 180.0f) / 2.0f);
}
#endif

void	remember_image(t_game *game, void *ptr)
{
	t_img	*new_node;

	if (ptr == NULL)
		return ;
	new_node = (t_img *)track_malloc(game, sizeof(t_img));
	new_node->ptr = ptr;
	new_node->next = game->img_head;
	game->img_head = new_node;
}
