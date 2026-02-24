/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:03:31 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/24 22:42:22 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
	mlx_hook(game->win, 6, 1L << 6, on_mouse_move, game);
	
}
#else

void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, on_key_down, game);
	mlx_hook(game->win, 3, 1L << 1, on_key_up, game);
	mlx_hook(game->win, 17, 0, on_window_close, game);
}

#endif

#ifdef BONUS

void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");
	load_wall_textures(game);
	load_door_texture(game);
	init_sprite_texture(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	setup_hooks(game);
}
#else

void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		graceful_exit(game, 1, __func__, "mlx_init failed.");
	load_wall_textures(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (game->win == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_window failed.");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (game->img == NULL)
		graceful_exit(game, 1, __func__, "mlx_new_image failed.");
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	setup_hooks(game);
}
#endif
