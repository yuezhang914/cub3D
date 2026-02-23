/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:36:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:27:30 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx-linux/mlx.h"
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include "func.h"

# define WIDTH 1280
# define HEIGHT 720
# define MINI_WIDTH 200
# define MINI_HEIGHT 200
# define FOV 66.0
# define WALL_BUFFER 0.20f
# define MOUSE_SENS 0.0025f
# define INF 1e30
# ifndef PI
#  define PI 3.14159265358979323846
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef struct s_img
{
	void					*ptr;
	struct s_img			*next;
}							t_img;

typedef struct s_coordinates
{
	float					x;
	float					y;
}							t_coords;

typedef struct s_int_coordinates
{
	int						x;
	int						y;
}							t_int_xy;

typedef struct s_texture
{
	void					*img_ptr;
	char					*path;
	char					*data;
	int						width;
	int						height;
	int						bpp;
	int						size_line;
	int						endian;
}							t_tex;

typedef struct s_dda
{
	int						map_x;
	int						map_y;
	int						step_x;
	int						step_y;
	int						side;
	float					delta_x;
	float					delta_y;
	float					side_x;
	float					side_y;
	float					perp_dist;
}							t_dda;

typedef struct s_player
{
	float					x;
	float					y;
	float					angle;

	int						map_x;
	int						map_y;

	int						key_up;
	int						key_down;
	int						key_left;
	int						key_right;
	int						key_rot_l;
	int						key_rot_r;

	float					move_speed;
	float					rot_speed;

	int						mouse_enabled;
	float					mouse_sens;
}							t_player;

typedef struct s_gnode
{
	void					*ptr;
	struct s_gnode			*next;
}							t_gnode;

typedef enum e_line_type
{
	EMPTY,
	NORTH,
	SOUTH,
	WEST,
	EAST,
	FLOOR,
	CEILING,
	MAP,
	WRONG
}							t_line_type;

typedef enum e_sprite_type
{
	SPR_TREE,
	SPR_TORCH,
	SPR_MONSTER,
	SPR_BARREL,
	SPR_COUNT
}							t_sprite_type;

typedef struct s_sprite
{
	float					x;
	float					y;
	float					dist;
	int						type;
	int						cur_frame;
	long long				last_update;
	float					angle;
}							t_sprite;

typedef struct s_sprite_config
{
	t_tex					*frames;
	int						frame_count;
	bool					is_animated;
	bool					is_directional;

	float					h_div;
	float					v_div;
	float					v_move;
}							t_sprite_config;

typedef struct s_sprite_manager
{
	t_sprite				*list;
	int						num;
	t_sprite_config			config[SPR_COUNT];
}							t_sprite_manager;

typedef struct s_game
{
	void					*mlx;
	void					*win;

	void					*img;
	char					*data;
	int						bpp;
	int						size_line;
	int						endian;

	t_tex					north;
	t_tex					south;
	t_tex					east;
	t_tex					west;

	int						floor_color;
	int						ceiling_color;

	char					**map;
	int						map_w;
	int						map_h;

	char					*entire_cubfile;
	char					**cubfile_lines;

	t_player				player;

	t_gnode					*track_head;

	t_img					*img_head;

	float					pix_per_unit;
	float					focal_length;

	t_sprite_manager		sprs;

	t_sprite_config			config[SPR_COUNT];
	float					time;

	t_tex					door;
	t_tex					door_open;
	unsigned char			**door_state;

	float					z_buffer[WIDTH];

}							t_game;

typedef struct s_render_vars
{
	int				x;			/* 屏幕横坐标：当前正在渲染的像素列索引 (0 到 WIN_WIDTH - 1) */
	int				start;		/* 绘制起点：墙体线在屏幕 Y 轴上的起始像素位置（防止超出屏幕顶部）*/
	int				end;		/* 绘制终点：墙体线在屏幕 Y 轴上的结束像素位置（防止超出屏幕底部）*/
	int				line_h;		/* 墙体高度：根据 perp_dist 计算出的该列墙体在屏幕上占据的像素总高度 */
	int				tex_x;		/* 纹理采样 X：撞击点在纹理图像上的横向坐标（决定取哪一列纹理像素）*/
	t_tex			*tex;		/* 纹理指针：指向当前墙面对应的纹理数据（NO/SO/WE/EA 之一）*/
	float			perp_dist;	/* 垂直距离：射线撞击点到玩家摄像机平面的垂直距离，用于修正鱼眼畸变 */
}					t_render_vars;

typedef struct s_sprite_render_vars
{
	int						sprite_h;
	int						sprite_w;
	int						draw_start_y;
	int						draw_end_y;
	int						draw_start_x;
	int						draw_end_x;
	int						screen_x;
	int						v_offset;
	int						type;
}							t_sprite_render_vars;

typedef struct s_draw_ctx
{
	t_game					*game;
	t_sprite_render_vars	v;

	float					trans_y;
	t_tex					*tex;

	int						sx;
	int						ex;

	int						sy;
	int						ey;

}							t_draw_ctx;

typedef struct s_ray
{
	float					dir_x;
	float					dir_y;
	float					perp_dist;
}							t_ray;

#endif
