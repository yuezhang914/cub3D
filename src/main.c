/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 12:54:21 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 21:00:59 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int argc, char **argv)
{
    t_game game;

    init_game(&game);
    module_parse(&game, argc, argv);

    /* --- 关键：必须先执行 setup_mlx --- */
    setup_mlx(&game);               // 2. 这里会执行 mlx_init()，给 game->mlx 赋值

    #ifdef BONUS
    /* --- 只有执行完 setup_mlx，game->mlx 才不是 NULL --- */
    handle_bonus_setup(&game);      // 3. 这里再加载精灵纹理和采集精灵
    #endif

    mlx_loop_hook(game.mlx, game_step, &game);
    mlx_loop(game.mlx);
    return (0);
}
/*
int main(int argc, char **argv)
{
    t_game game;

    if (argc != 2)
        return (print_error("main", "Usage: ./cub3D map.cub"), 1);

    // 1. 初始化结构体（清零指针，设置默认速度）
    init_game(&game);

    // 2. 解析 .cub 文件（获取贴图路径、颜色、地图数组）
    module_parse(&game, argc, argv);

    // 3. 初始化 MLX 连接、窗口、主画布，并加载墙壁贴图
    setup_mlx(&game); // 执行完这一步，game.mlx 才有值

    #ifdef BONUS
    // 4. 加载精灵纹理（现在 game.mlx 不是 NULL 了，不会报错）
    init_sprite_texture(&game);
    // 5. 从地图中提取精灵坐标
    collect_sprites(&game);
    #endif

    // 6. 开启游戏循环
    mlx_loop_hook(game.mlx, game_step, &game);
    mlx_loop(game.mlx);

    return (0);
}
*/
// void init_game(t_game *game)
// {
// 	game->mlx = mlx_init();
// 	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
// 	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
// 	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			// &game->endian);
// 	game->ceiling_color = -1;
// 	game->floor_color = -1;
// 	game->north.path = NULL;
// 	game->south.path = NULL;
// 	game->east.path = NULL;
// 	game->west.path = NULL;

// 	init_player(&game->player);
// }

// void load_texture(t_game *game, t_tex *tex, char *path)
// {
// 	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, path, &tex->width,
			// &tex->height);
// 	if (!tex->img_ptr)
// 	{
// 		printf("Error: Failed to load texture at %s\n", path);
// 		exit(1);
// 	}
// 	tex->addr = (int *)mlx_get_data_addr(tex->img_ptr, &tex->bpp,
// 											&tex->size_line, &tex->endian);
// }
// int main(int argc, char **argv)
// {
// 	t_game game;

// 	init_game(&game);
// 	module_parse(&game, argc, argv);
// 	load_texture(&game, &game.north, game.north.path);
// 	load_texture(&game, &game.south, game.south.path);
// 	load_texture(&game, &game.west, game.west.path);
// 	load_texture(&game, &game.east, game.east.path);

// 	// 注册钩子函数处理键盘输入
// 	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
// 	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
// 	// 启动主渲染循环
// 	mlx_loop_hook(game.mlx, draw_loop, &game);
// 	mlx_loop(game.mlx);
// 	return (0);
// }
