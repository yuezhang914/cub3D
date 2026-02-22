/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:07:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 21:55:47 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数名：load_texture
** 作用：把一张 xpm 图片加载为 MLX image，并取得像素数据地址（之后渲染会直接读像素）
** 参数：
**   game：
**     - 提供 game->mlx（mlx 环境指针）
**     - 用于 graceful_exit（统一错误退出）
**     - 用于 remember_image（记录 image 以便释放）
**   tex：贴图结构体，至少包含：
**     - path：xpm 文件路径（从 .cub 解析得到）
**     - img_ptr：mlx image 指针（此函数会填）
**     - width/height：图片尺寸（mlx 会回填）
**     - data：像素数组指针（此函数会填）
**     - bpp/size_line/endian：像素格式信息（mlx_get_data_addr 会回填）
** 主要逻辑：
**   1) 若 path 为空：说明 cub 没配置或解析失败，直接报错
**   2) mlx_xpm_file_to_image：加载 xpm 成 image
**   3) 若 img_ptr 为空：说明文件不存在或坏了，打印路径并退出
**   4) remember_image：把 img_ptr 记录下来，便于统一释放
**   5) mlx_get_data_addr：得到 data 指针，后面“取纹理颜色”要靠它
** 调用者：
**   load_wall_textures()：加载四面墙
**   BONUS：门贴图、sprite 贴图的加载函数会用同样模式
*/
void	load_texture(t_game *game, t_tex *tex)
{
	char	cwd[1024];

	/* cwd 在你这份代码里只是为了调试（看当前工作目录） */
	getcwd(cwd, sizeof(cwd));

	if (!tex->path)
		graceful_exit(game, 1, __func__, "Texture path is missing.");

	/* 读取 xpm，得到 image 指针和图片宽高 */
	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, tex->path,
			&tex->width, &tex->height);

	if (tex->img_ptr == NULL)
	{
		printf("Error: Could not find or open [%s]\n", tex->path);
		graceful_exit(game, 1, __func__, "XPM loading failed.");
	}

	/* 记录 image，用于退出时统一释放 */
	remember_image(game, tex->img_ptr);

	/* 拿到像素数据地址：后面渲染会从 tex->data 里取颜色 */
	tex->data = mlx_get_data_addr(tex->img_ptr,
			&tex->bpp, &tex->size_line, &tex->endian);
}

/*
** 函数名：load_wall_textures
** 作用：加载四面墙贴图（北、东、南、西）
** 参数：
**   game：里面有 game->north/east/south/west 四个贴图结构体
** 主要逻辑：
**   对四个方向依次调用 load_texture
** 调用者：
**   setup_mlx()（setup.c）
*/
void	load_wall_textures(t_game *game)
{
	load_texture(game, &game->north);
	load_texture(game, &game->east);
	load_texture(game, &game->south);
	load_texture(game, &game->west);
}
