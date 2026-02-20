/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:07:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/20 19:04:24 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
** 函数：load_texture
** 作用：根据 tex->path 把 xpm 加载成 MLX image，并取出像素数据地址供渲染使用。
** 参数：
**   game：主结构体（提供 mlx，并用于 graceful_exit、remember_image）
**   tex ：贴图结构体（必须先填好 path；函数会填 img_ptr/width/height/data 等）
** 用在哪：
**   load_wall_textures() 与 load_anim() 中都会调用它。
*/
void load_texture(t_game *game, t_tex *tex)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	if (!tex->path)
		graceful_exit(game, 1, __func__, "Texture path is missing.");

	// 1. 加载图片
	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, tex->path, &tex->width,
										 &tex->height);

	// 2. 增强报错信息，方便调试是哪张图没找到
	if (tex->img_ptr == NULL)
	{
		printf("Error: Could not find or open [%s]\n", tex->path); // 调试利器
		graceful_exit(game, 1, __func__, "XPM loading failed.");
	}

	// 3. 内存管理
	remember_image(game, tex->img_ptr);

	// 4. 获取像素地址
	tex->data = mlx_get_data_addr(tex->img_ptr, &tex->bpp, &tex->size_line,
								  &tex->endian);
}

/*
** 函数：load_wall_textures
** 作用：加载四个方向墙贴图（NO/EA/SO/WE），这属于 mandatory 必做。
** 参数：
**   game：主结构体（里面的 north/east/south/west 已在 parse 阶段写好 path）
** 用在哪：
**   setup_mlx() 里调用，窗口创建后立刻加载贴图。
*/
void load_wall_textures(t_game *game)
{
	load_texture(game, &game->north);
	load_texture(game, &game->east);
	load_texture(game, &game->south);
	load_texture(game, &game->west);
}
