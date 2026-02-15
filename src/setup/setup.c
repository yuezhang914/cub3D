/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:05:48 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 14:49:13 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	init_game(t_game *game)
{
	float	fov_rad;

	ft_bzero(game, sizeof(t_game));
	game->ceiling_color = -1;
	game->floor_color = -1;
	fov_rad = FOV * (PI / 180.0f);
	game->focal_length = 2.0 * tan(fov_rad / 2.0);
	game->pix_per_unit = (float)MINI_HEIGHT / (DISTANCE_SEEN * 2.0f);
	init_player(&game->player);
}

void	remember_image(t_game *game, void *ptr)
{
	t_img	*new_img;

	new_img = track_malloc(game, sizeof(t_img));
	new_img->ptr = ptr;
	new_img->next = game->img_head;
	game->img_head = new_img;
}

void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, key_press, &game->player);
	mlx_hook(game->win, 3, 1L << 1, key_release, &game->player);
}

void	setup_mlx(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	load_wall_textures(game);
	setup_hooks(game);
}
