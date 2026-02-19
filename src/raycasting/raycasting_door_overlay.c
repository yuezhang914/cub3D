/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_door_overlay.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 12:53:28 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 12:59:04 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

#ifdef BONUS

/*
** 函数：door_find_open_door（static）
** 作用：沿射线 DDA 查找第一个“开着的门”，找到就停
** 参数：game/d/r_dir_x/r_dir_y
** 返回：1=找到开门 0=没找到（或先遇墙/越界）
** 逻辑：遇墙直接失败；遇 D 且 door_state==1 则成功
** 在哪调用：get_open_door_overlay_vars()
*/
static int	door_find_open_door(t_game *game, t_dda *d,
					float r_dir_x, float r_dir_y)
{
	while (1)
	{
		ray_step_once(d);
		if (ray_outside_map(game, d))
			return (0);
		if (game->map[d->map_y][d->map_x] == '1')
			return (0);
		if (game->map[d->map_y][d->map_x] == 'D')
		{
			if (game->door_state
				&& game->door_state[d->map_y][d->map_x] == 1)
			{
				d->perp_dist = ray_get_perp_dist(d);
				(void)door_ray_blocks(game, d, r_dir_x, r_dir_y);
				return (1);
			}
			return (0);
		}
	}
	return (0);
}

/*
** 函数：door_make_overlay_vars（static）
** 作用：把“开门命中信息”转成一列 overlay 的渲染参数
** 参数：game/d/r_dir_x/r_dir_y/i
** 返回：t_render_vars（tex=door_open）
** 在哪调用：get_open_door_overlay_vars()
*/
static t_render_vars	door_make_overlay_vars(t_game *game, t_dda *d,
						float r_dir_x, float r_dir_y)
{
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = &game->door_open;
	v.perp_dist = d->perp_dist;
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;
	v.line_h = (int)(HEIGHT / v.perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);
	v.tex_x = ray_calc_tex_x(game, d, v.tex, v.perp_dist, r_dir_x, r_dir_y);
	return (v);
}

/*
** 函数：get_open_door_overlay_vars
** 作用：找到“开着的门”并生成门框 overlay 的渲染参数
** 参数：game/r_dir_x/r_dir_y/i
** 返回：t_render_vars；若没有开门则返回空结构（tex==NULL）
** 逻辑：
**   1) init_dda
**   2) 找到第一个开门
**   3) 生成 overlay vars（tex=door_open）
** 在哪调用：
**   render_column()：画完墙后叠加门框（透明像素跳过）
*/
t_render_vars	get_open_door_overlay_vars(t_game *game,
					float r_dir_x, float r_dir_y, int i)
{
	t_dda			d;
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	(void)i;
	if (!game || !game->door_state)
		return (v);
	ray_init_dda(game, &d, r_dir_x, r_dir_y);
	if (!door_find_open_door(game, &d, r_dir_x, r_dir_y))
		return (v);
	v = door_make_overlay_vars(game, &d, r_dir_x, r_dir_y);
	return (v);
}

#endif
