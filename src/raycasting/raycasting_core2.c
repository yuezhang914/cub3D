/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core 2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:58:02 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/*
** 函数：ray_step_once
** 作用：执行一次 DDA 步进（更新 map_x/map_y 与 side）
** 参数：d：DDA 状态
** 逻辑：比较 side_x/side_y，走更近的那一格
** 在哪调用：ray_run_dda_until_hit(), door_find_open_door()
*/
void	ray_step_once(t_dda *d)
{
	if (d->side_x < d->side_y)
	{
		d->side_x += d->delta_x;
		d->map_x += d->step_x;
		d->side = 0;
		return ;
	}
	d->side_y += d->delta_y;
	d->map_y += d->step_y;
	d->side = 1;
}

/*
** 函数：ray_outside_map
** 作用：判断 DDA 是否跑出地图
** 参数：game/d
** 返回：1=越界 0=未越界
** 在哪调用：ray_run_dda_until_hit(), door_find_open_door()
*/
int	ray_outside_map(t_game *game, t_dda *d)
{
	if (d->map_x < 0 || d->map_x >= game->map_w)
		return (1);
	if (d->map_y < 0 || d->map_y >= game->map_h)
		return (1);
	return (0);
}

/*
** 函数：ray_pick_texture
** 作用：根据命中格子和方向选择纹理
** 参数：game/d/r_dx/r_dy
** 返回：纹理指针
** 在哪调用：ray_make_render_vars()
*/
t_tex	*ray_pick_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
#ifdef BONUS
	if (game->map[d->map_y][d->map_x] == 'D')
	{
		if (game->door_state && game->door_state[d->map_y][d->map_x] == 1)
			return (&game->door_open);
		return (&game->door);
	}
#endif
	if (d->side == 0 && r_dx > 0.0f)
		return (&game->east);
	if (d->side == 0)
		return (&game->west);
	if (r_dy > 0.0f)
		return (&game->south);
	return (&game->north);
}

/*
** 函数：ray_get_perp_dist
** 作用：根据 side 计算垂直距离 perp_dist
** 参数：d
** 返回：perp_dist
** 在哪调用：ray_make_render_vars(), door_make_overlay_vars()
*/
float	ray_get_perp_dist(t_dda *d)
{
	if (d->side == 0)
		return (d->side_x - d->delta_x);
	return (d->side_y - d->delta_y);
}

/*
** 函数：ray_calc_tex_x
** 作用：计算纹理 x 坐标 tex_x（保证不越界）
** 参数：game/d/tex/perp/r_dx/r_dy
** 返回：tex_x
** 在哪调用：ray_make_render_vars(), door_make_overlay_vars()
*/
int	ray_calc_tex_x(t_game *game, t_dda *d, t_tex *tex, float perp,
			float r_dx, float r_dy)
{
	float	hit;
	int		tex_x;

	if (d->side == 0)
		hit = game->player.y + perp * r_dy;
	else
		hit = game->player.x + perp * r_dx;
	hit -= floor(hit);
	tex_x = (int)(hit * (float)tex->width);
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_x < 0)
		tex_x = 0;
	if ((d->side == 0 && r_dx < 0.0f) || (d->side == 1 && r_dy > 0.0f))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}
