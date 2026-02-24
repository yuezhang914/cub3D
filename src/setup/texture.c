/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:07:47 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/23 22:24:14 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	load_texture(t_game *game, t_tex *tex)
{
	if (!tex->path)
		graceful_exit(game, 1, __func__, "Texture path is missing.");
	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, tex->path, &tex->width,
			&tex->height);
	if (tex->img_ptr == NULL)
	{
		printf("Error: Could not find or open [%s]\n", tex->path);
		graceful_exit(game, 1, __func__, "XPM loading failed.");
	}
	remember_image(game, tex->img_ptr);
	tex->data = mlx_get_data_addr(tex->img_ptr, &tex->bpp, &tex->size_line,
			&tex->endian);
}

void	load_wall_textures(t_game *game)
{
	load_texture(game, &game->north);
	load_texture(game, &game->east);
	load_texture(game, &game->south);
	load_texture(game, &game->west);
}
