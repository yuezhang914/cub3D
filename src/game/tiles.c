/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:43:57 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:55:01 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** 函数：hit_direction
** 作用：根据命中点 wall_hit 判断撞到墙的哪一面（北/东/南/西）。
** 参数：
**   game：总结构体（用玩家位置判断左右/上下）
**   wall_hit：射线撞墙的坐标
** 返回：
**   t_dir（north/east/south/west）
** 用在哪：
**   贴图选择、以及计算“命中的是哪个格子”的坐标。
*/
t_dir	hit_direction(t_game *game, t_coords wall_hit)
{
	if (is_round(wall_hit.x))
	{
		if (game->player.x > wall_hit.x)
			return (west);
		return (east);
	}
	if (game->player.y > wall_hit.y)
		return (north);
	return (south);
}

/*
** 函数：hit_tile_coords
** 作用：根据命中点和方向，算出“被打中的地图格子坐标(x,y)”。
** 参数：
**   game：用来做边界修正
**   hit：命中点（通常是 next_wall_hit / next_grid_hit 返回）
**   dir：命中方向（hit_direction 得到）
** 返回：
**   t_int_xy：格子坐标
** 用在哪：
**   取地图字符（墙/门/道具），或对某格子做交互。
*/
t_int_xy	hit_tile_coords(t_game *game, t_coords hit, t_dir dir)
{
	t_int_xy	c;

	if (dir == north || dir == south)
	{
		c.y = roundf(hit.y);
		c.x = truncf(hit.x);
	}
	if (dir == west || dir == east)
	{
		c.y = truncf(hit.y);
		c.x = roundf(hit.x);
	}
	if (dir == north)
		c.y--;
	else if (dir == west)
		c.x--;
	if (c.y >= game->map_h)
		c.y = game->map_h - 1;
	return (c);
}

/*
** 函数：hit_tile_type
** 作用：直接返回“命中的格子里是什么字符”（比如 '1' '0' 'P' 等）。
** 参数：
**   game：总结构体（用 game->map）
**   hit：命中点
**   dir：方向
** 返回：
**   地图字符（char）
** 用在哪：
**   交互逻辑：判断打到的是宝可球/火箭队/可切割物等。
*/
char	hit_tile_type(t_game *game, t_coords hit, t_dir dir)
{
	t_int_xy	c;

	c = hit_tile_coords(game, hit, dir);
	return (game->map[c.y][c.x]);
}

/*
** 函数：adjacent_tile_ahead
** 作用：计算“玩家面前紧贴着的格子坐标”（用于交互：挖/切/开门）。
** 参数：
**   game：总结构体
** 返回：
**   t_int_xy：玩家正前方格子
** 用在哪：
**   try_try_remove_obstacle() 这类交互函数可以用它。
*/
t_int_xy	adjacent_tile_ahead(t_game *game)
{
	t_coords	hit;
	t_dir		dir;

	hit = next_grid_hit((t_coords){game->player.x, game->player.y},
			game->player.angle);
	dir = hit_direction(game, hit);
	return (hit_tile_coords(game, hit, dir));
}

/*
** 函数：find_tile
** 作用：在整张地图里寻找某个字符第一次出现的位置。
** 参数：
**   game：总结构体（用 map_w/map_h/map）
**   c：要找的字符
** 返回：
**   找到：坐标(x,y)
**   找不到：(-1,-1)
** 用在哪：
**   bonus：找某个特殊物体位置（例如火箭队/道具）。
*/
t_int_xy	find_tile(t_game *game, char c)
{
	t_int_xy	p;

	p.y = 0;
	while (p.y < game->map_h)
	{
		p.x = 0;
		while (p.x < game->map_w)
		{
			if (game->map[p.y][p.x] == c)
				return (p);
			p.x++;
		}
		p.y++;
	}
	p.x = -1;
	p.y = -1;
	return (p);
}
