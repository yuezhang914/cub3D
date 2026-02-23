/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_core 2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:00:14 by weiyang           #+#    #+#             */
/*   Updated: 2026/02/19 12:58:02 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ray_step_once(t_dda *d)
{
	/* 1. 比较射线到下一条垂直网格线 (side_x) 和下一条水平网格线 (side_y) 的距离 */
	if (d->side_x < d->side_y)
	{
		/* 如果 X 方向距离更近，则射线在 X 轴上前进一格 */
		/* 更新 side_x，累加一个单位跨度 delta_x */
		d->side_x += d->delta_x;
		/* 移动网格坐标，step_x 为 1 (向右) 或 -1 (向左) */
		d->map_x += d->step_x;
		/* 记录撞击性质：0 表示撞击的是垂直墙面 (东西向) */
		d->side = 0;
		return ;
	}
	/* 2. 否则，如果 Y 方向距离更近（或相等），则射线在 Y 轴上前进一格 */
	/* 更新 side_y，累加一个单位跨度 delta_y */
	d->side_y += d->delta_y;
	/* 移动网格坐标，step_y 为 1 (向下) 或 -1 (向上) */
	d->map_y += d->step_y;
	/* 记录撞击性质：1 表示撞击的是水平墙面 (南北向) */
	d->side = 1;
}

int	ray_outside_map(t_game *game, t_dda *d)
{
	/* 1. 检查 X 坐标是否超出水平边界 */
	/* 如果 map_x 小于 0 或者 大于等于地图的宽度，说明射线已出界 */
	if (d->map_x < 0 || d->map_x >= game->map_w)
		return (1);

	/* 2. 检查 Y 坐标是否超出垂直边界 */
	/* 如果 map_y 小于 0 或者 大于等于地图的高度，说明射线已出界 */
	if (d->map_y < 0 || d->map_y >= game->map_h)
		return (1);

	/* 3. 若以上条件都不满足，说明射线仍在合法地图范围内 */
	return (0);
}

t_tex	*ray_pick_texture(t_game *game, t_dda *d, float r_dx, float r_dy)
{
	/* 1. 检查特殊物体：如果撞击的是门 ('D') */
	if (game->map[d->map_y][d->map_x] == 'D')
	{
		/* 根据门的状态（开启/关闭）返回对应的纹理 */
		/* 这里 game->door_state 记录了门是开(1)还是关(0) */
		if (game->door_state && game->door_state[d->map_y][d->map_x] == 1)
			return (&game->door_open);
		return (&game->door);
	}
	/* 2. 判断撞击面：如果 side == 0，说明撞击的是垂直网格线（东西方向的墙面） */
	if (d->side == 0)
	{
		/* 如果光线水平方向分量 > 0，说明光线向右飞，撞到的是墙的西面，显示东侧纹理 */
		if (r_dx > 0.0f)
			return (&game->east);
		/* 反之，光线向左飞，撞到墙的东面，显示西侧纹理 */
		return (&game->west);
	}
	/* 3. 如果 side == 1，说明撞击的是水平网格线（南北方向的墙面） */
	/* 如果光线垂直方向分量 > 0，说明光线向下飞，撞到墙的北面，显示南侧纹理 */
	if (r_dy > 0.0f)
		return (&game->south);
	/* 反之，光线向上飞，撞到墙的南面，显示北侧纹理 */
	return (&game->north);
}

float	ray_get_perp_dist(t_dda *d)
{
	/* 1. 如果 side == 0，表示光线撞击的是垂直网格线（东西方向的墙） */
	if (d->side == 0)
	{
		/* 此时 side_x 包含了最后一次跳跃后的总长度。 */
		/* 由于 DDA 在撞墙后还会多跳一个 delta_x，所以需要减去它， */
		/* 从而得到从玩家到墙面的投影距离。 */
		return (d->side_x - d->delta_x);
	}
	/* 2. 如果 side == 1，表示光线撞击的是水平网格线（南北方向的墙） */
	/* 同理，减去最后一个 delta_y 得到垂直投影距离。 */
	return (d->side_y - d->delta_y);
}

int	ray_calc_tex_x(t_game *game, t_dda *d, t_tex *tex, t_ray ray)
{
	float	hit;
	int		tex_x;

	if (d->side == 0)
		hit = game->player.y + ray.perp_dist * ray.dir_y;
	else
		hit = game->player.x + ray.perp_dist * ray.dir_x;
	hit -= floor((double)hit);
	if (hit < 0)
		hit += 1.0;
	tex_x = (int)(hit * (float)tex->width);
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_x < 0)
		tex_x = 0;
	if ((d->side == 0 && ray.dir_x < 0.0) || (d->side == 1 && ray.dir_y > 0.0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}
