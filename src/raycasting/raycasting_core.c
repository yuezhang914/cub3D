/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 13:04:44 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：ray_init_dda
** 作用：初始化 DDA 基础数据（map坐标、delta、step、side起始距离）
** 参数：game/d/r_dir_x/r_dir_y
** 逻辑：delta 用 if 分支（禁用 ?:），step/side 用 helper 设置
** 在哪调用：get_render_vars(), get_open_door_overlay_vars()
*/
void	ray_init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
	d->map_x = (int)game->player.x;
	d->map_y = (int)game->player.y;
	if (r_dir_x == 0.0f)
		d->delta_x = 1e30f;
	else
		d->delta_x = fabsf(1.0f / r_dir_x);
	if (r_dir_y == 0.0f)
		d->delta_y = 1e30f;
	else
		d->delta_y = fabsf(1.0f / r_dir_y);
	ray_set_step_x(game, d, r_dir_x);
	ray_set_step_y(game, d, r_dir_y);
}

/*
** 函数：ray_set_step_x
** 作用：设置 x 方向的 step 和 side_x
** 参数：game/d/r_dir_x
** 逻辑：向左 step_x=-1，否则 step_x=1
** 在哪调用：ray_init_dda()
*/
void	ray_set_step_x(t_game *game, t_dda *d, float r_dir_x)
{
	if (r_dir_x < 0.0f)
	{
		d->step_x = -1;
		d->side_x = (game->player.x - d->map_x) * d->delta_x;
		return ;
	}
	d->step_x = 1;
	d->side_x = (d->map_x + 1.0f - game->player.x) * d->delta_x;
}

/*
** 函数：ray_set_step_y
** 作用：设置 y 方向的 step 和 side_y
** 参数：game/d/r_dir_y
** 逻辑：向上 step_y=-1，否则 step_y=1
** 在哪调用：ray_init_dda()
*/
void	ray_set_step_y(t_game *game, t_dda *d, float r_dir_y)
{
	if (r_dir_y < 0.0f)
	{
		d->step_y = -1;
		d->side_y = (game->player.y - d->map_y) * d->delta_y;
		return ;
	}
	d->step_y = 1;
	d->side_y = (d->map_y + 1.0f - game->player.y) * d->delta_y;
}

/*
** 函数：ray_run_dda_until_hit
** 作用：DDA 循环直到命中墙/门板，或越界
** 参数：game/d/r_dir_x/r_dir_y
** 返回：1=命中可画物体 0=越界
** 在哪调用：get_render_vars()
*/
int	ray_run_dda_until_hit(t_game *game, t_dda *d, float r_dir_x, float r_dir_y)
{
#ifndef BONUS
	(void)r_dir_x;
	(void)r_dir_y;
#endif
	while (1)
	{
		ray_step_once(d);
		if (ray_outside_map(game, d))
			return (0);
		if (game->map[d->map_y][d->map_x] == '1')
			return (1);
#ifdef BONUS
		if (game->map[d->map_y][d->map_x] == 'D')
			if (door_ray_blocks(game, d, r_dir_x, r_dir_y))
				return (1);
#endif
	}
	return (0);
}

/*
** 函数：ray_make_render_vars
** 作用：从 DDA 命中信息生成渲染参数（纹理、距离、柱高、tex_x）
** 参数：game/d/r_dir_x/r_dir_y
** 返回：t_render_vars
** 在哪调用：get_render_vars()
*/
t_render_vars	ray_make_render_vars(t_game *game, t_dda *d, float r_dir_x,
		float r_dir_y)
{
	t_render_vars	v;

	ft_bzero(&v, sizeof(t_render_vars));
	v.tex = ray_pick_texture(game, d, r_dir_x, r_dir_y);
	v.perp_dist = ray_get_perp_dist(d);
	if (v.perp_dist < 0.1f)
		v.perp_dist = 0.1f;
	v.line_h = (int)(HEIGHT / v.perp_dist);
	v.start = (HEIGHT / 2) - (v.line_h / 2);
	v.end = (HEIGHT / 2) + (v.line_h / 2);
	v.tex_x = ray_calc_tex_x(game, d, v.tex, v.perp_dist, r_dir_x, r_dir_y);
	return (v);
}
