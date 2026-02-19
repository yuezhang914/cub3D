/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:07:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/18 19:53:52 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "func.h"

// /*
// ** 函数：make_anim_path（static）
// ** 作用：生成动画帧贴图路径：base + "_" + 三位数字 + ".xpm"
// **      例：base="textures/cut" i=7  -> "textures/cut_007.xpm"
// ** 参数：
// **   game：用于 track_malloc
// **   base：基础路径（不含 _xxx.xpm）
// **   i   ：帧编号
// ** 返回：
// **   新分配的路径字符串
// ** 用在哪：
// **   load_anim() 内部，用它替代 sprintf（避免用不允许的函数）。
// */
// static char	*make_anim_path(t_game *game, char *base, int i)
// {
// 	char	*res;
// 	int		len;

// 	len = (int)ft_strlen(base) + 8;
// 	res = track_malloc(game, (len + 1) * sizeof(char));
// 	ft_memcpy(res, base, ft_strlen(base));
// 	res[ft_strlen(base)] = '_';
// 	res[ft_strlen(base) + 1] = (char)('0' + (i / 100) % 10);
// 	res[ft_strlen(base) + 2] = (char)('0' + (i / 10) % 10);
// 	res[ft_strlen(base) + 3] = (char)('0' + i % 10);
// 	ft_memcpy(res + ft_strlen(base) + 4, ".xpm", 4);
// 	res[len] = '\0';
// 	return (res);
// }

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
	printf("Current working dir: %s\n", cwd);

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

// /*
// ** 函数：load_anim
// ** 作用：加载一个动画：根据 base path 生成每一帧的 xpm 路径并逐帧加载。
// ** 参数：
// **   game  ：主结构体
// **   anim  ：要被填充的动画对象（输出：frames/nbr_frames/duration_ms）
// **   params：动画参数（输入：帧数与时长）
// **   path  ：基础路径（例如 "textures/cut"）
// ** 用在哪：
// **   bonus：需要动画精灵/特效时调用（mandatory 可以不调用它）。
// */
// void	load_anim(t_game *game, t_anim *anim, t_anim params, char *path)
// {
// 	int		i;
// 	char	*current_path;

// 	anim->nbr_frames = params.nbr_frames;
// 	anim->duration_ms = params.duration_ms;
// 	anim->frames = track_malloc(game, sizeof(t_tex) * params.nbr_frames);
// 	i = 0;
// 	while (i < params.nbr_frames)
// 	{
// 		current_path = make_anim_path(game, path, i);
// 		anim->frames[i].path = current_path;
// 		load_texture(game, &anim->frames[i]);
// 		i++;
// 	}
// }
