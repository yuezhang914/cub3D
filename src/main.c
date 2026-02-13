/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 12:54:21 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 21:27:52 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cube3d.h"

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	game->ceiling_color = -1;
	game->floor_color = -1;
	game->north.path = NULL;
	game->south.path = NULL;
	game->east.path = NULL;
	game->west.path = NULL;

	init_player(&game->player);
}

void load_texture(t_game *game, t_tex *tex, char *path)
{
	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img_ptr)
	{
		printf("Error: Failed to load texture at %s\n", path);
		exit(1);
	}
	tex->addr = (int *)mlx_get_data_addr(tex->img_ptr, &tex->bpp,
										 &tex->size_line, &tex->endian);
}
int main(int argc, char **argv)
{
	t_game game;
	
	init_game(&game);
	module_parse(&game, argc, argv);
	load_texture(&game, &game.north, game.north.path);
	load_texture(&game, &game.south, game.south.path);
	load_texture(&game, &game.west, game.west.path);
	load_texture(&game, &game.east, game.east.path);

	// 注册钩子函数处理键盘输入
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	// 启动主渲染循环
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}
