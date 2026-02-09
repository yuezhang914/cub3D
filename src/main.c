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

/**
 * 初始化图像缓冲区和地图数据
 */
void init_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    game->map = get_map();
    init_player(&game->player);
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
