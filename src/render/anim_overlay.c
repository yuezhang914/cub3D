/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim_overlay.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:06:52 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/08 01:15:25 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static t_anim_queue	*find_anim(t_game *game, t_int_xy tile);
static int		current_frame_or_end(t_game *game, t_anim_queue *a);

/*
** 函数：superpose_anim
** 作用：在原本墙面颜色上“叠加动画像素”（如果动画像素不是 0，就覆盖原颜色）。
** 参数：
**   game：提供动画队列 anim_head、地图 map、以及取像素函数 tex_sample
**   tile：当前命中的地图格子坐标
**   relative：墙面上的相对坐标(0..1)，用于在动画贴图里取对应像素
**   color：输入/输出颜色指针（动画覆盖后会改它）
** 用在哪：
**   你如果在 wall_tex_color 之后想叠动画，就调用它。
*/
void	superpose_anim(t_game *game, t_int_xy tile, t_coords relative, int *color)
{
	t_anim_queue	*a;
	int		frame;
	t_tex		*tex;
	int		anim_color;

	if (game->map[tile.y][tile.x] == '1')
		return ;
	a = find_anim(game, tile);
	if (a == NULL)
		return ;
	frame = current_frame_or_end(game, a);
	if (frame < 0)
		return ;
	tex = &a->anim->frames[frame];
	anim_color = tex_sample(tex, relative);
	if (anim_color != 0)
		*color = anim_color;
}

static t_anim_queue	*find_anim(t_game *game, t_int_xy tile)
{
	t_anim_queue	*cur;

	cur = game->anim_head;
	while (cur)
	{
		if (cur->coords.x == tile.x && cur->coords.y == tile.y)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/*
** 函数：current_frame_or_end（static）
** 作用：根据“动画开始时间”计算现在应该显示第几帧；如果动画已结束则移除。
** 参数：
**   game：用于 anim_remove 和改地图字符
**   a：当前动画节点
** 返回：
**   >=0：当前帧编号
**   -1 ：动画结束（已经移除）
*/
static int	current_frame_or_end(t_game *game, t_anim_queue *a)
{
	struct timeval	now;
	int		passed;
	int		dur;

	dur = a->anim->duration_ms;
	gettimeofday(&now, NULL);
	passed = time_difms(&now, &a->start);
	if (passed > dur)
	{
		game->map[a->coords.y][a->coords.x] = '0';
		anim_remove(game, a);
		return (-1);
	}
	return ((int)((float)passed / (float)dur * a->anim->nbr_frames));
}
