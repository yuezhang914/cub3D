/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 12:54:21 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/23 23:27:31 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_game	game;

	init_game(&game);
	module_parse(&game, argc, argv);
	setup_mlx(&game);
	mlx_loop_hook(game.mlx, game_step, &game);
	mlx_loop(game.mlx);
	return (0);
}
