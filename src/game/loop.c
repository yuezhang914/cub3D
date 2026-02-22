/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:42:58 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:12:51 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef BONUS

/*
** 函数名：handle_bonus_rendering（static，BONUS）
** 作用：处理 bonus 的额外渲染内容（sprite + 小地图）
**
** 参数：
**   game：提供 sprite 列表与 minimap 数据
**
** 主要逻辑：
**   1) 如果 sprs.num > 0：
**      - sort_sprites：按距离排序（远的先画或近的先画，取决于你项目实现）
**      - render_sprites：绘制 sprite
**   2) render_minimap：绘制小地图
**
** 调用者：
**   game_step()：墙面渲染 draw_loop 之后调用
*/
static void	handle_bonus_rendering(t_game *game)
{
	if (game->sprs.num > 0)
	{
		sort_sprites(game);
		render_sprites(game);
	}
	render_minimap(game);
}

#else

/*
** 函数名：handle_bonus_rendering（static，非 BONUS）
** 作用：基础版没有 sprite/minimap，因此什么都不做
**
** 参数：
**   game：不用，避免警告
**
** 调用者：
**   game_step()
*/
static void	handle_bonus_rendering(t_game *game)
{
	(void)game;
}
#endif

/*
** 函数名：game_step
** 作用：每一帧的主循环函数：更新→绘制→显示
**
** 参数：
**   game：游戏总状态（玩家、画布、mlx 等）
**
** 主要逻辑（每帧流水线）：
**   1) time += 0.016：
**      - 这里相当于假设每帧固定 16ms（约 60FPS）
**      - 注意：这不是精确计时，但实现简单
**   2) update_player：
**      - 根据 input.c 记录的 key 状态更新玩家位置/角度
**   3) clear_image：
**      - 清空离屏画布（不清会残影）
**   4) draw_loop：
**      - 绘制世界主体（墙等）
**   5) handle_bonus_rendering：
**      - BONUS：sprite + 小地图
**   6) mlx_put_image_to_window：
**      - 把离屏画布一次性贴到窗口，避免闪烁（双缓冲思路）
**
** 调用者：
**   mlx_loop_hook 或 mlx_loop 的回调（你项目其他地方绑定）
*/
int	game_step(t_game *game)
{
	game->time += 0.016f;
	update_player(game);
	clear_image(game);
	draw_loop(game);
	handle_bonus_rendering(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

/*
** 函数名：on_window_close
** 作用：点击窗口右上角 X 时的回调：优雅退出
**
** 参数：
**   param：mlx_hook 传进来的参数（这里强转为 t_game*）
**
** 主要逻辑：
**   graceful_exit(game, 0, NULL, NULL)
**
** 返回：
**   1：通常表示事件已处理
**
** 调用者：
**   mlx_hook(window, 17, ...) 绑定（你项目其他地方）
*/
int	on_window_close(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	graceful_exit(game, 0, NULL, NULL);
	return (1);
}
