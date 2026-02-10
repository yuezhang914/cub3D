/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:45:06 by kweihman          #+#    #+#             */
/*   Updated: 2026/02/07 18:01:46 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"


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

void init_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    game->map = get_map();
    init_player(&game->player);

    
    load_texture(game, &game->north, "src/texture/north.xpm");
    load_texture(game, &game->south, "src/texture/south.xpm");
    load_texture(game, &game->east, "src/texture/east.xpm");
    load_texture(game, &game->west, "src/texture/west.xpm");
	game->ceiling_color = 0xC0C0C0;
	game->floor_color = 0xDEB887;
}


int main(void)
{
	t_game game;

	init_game(&game);
	// 注册钩子函数处理键盘输入
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	// 启动主渲染循环
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}
