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

	init_game(&game);				 // 1. 初始化结构体
	module_parse(&game, argc, argv); // 解析地图文件

	setup_mlx(&game); // 2. mlx_init + 创建窗口 + 图像
	mlx_loop_hook(game.mlx, game_step, &game);
	mlx_loop(game.mlx);
	return 0;
}
