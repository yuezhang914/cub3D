/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:36:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 12:36:58 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include "../minilibx-linux/mlx.h"
#include <stdbool.h>

/* ========== 常量（按你项目已有的即可） ========== */
#define WIDTH 1280
#define HEIGHT 720
#define MINI_WIDTH 200
#define MINI_HEIGHT 200
#define FOV 66.0
#define WALL_BUFFER 0.20f
#define MOUSE_SENS 0.0025f

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

/* ===================== 基础类型 ===================== */
/*
** 结构体：t_img
** 作用：记录“需要 mlx_destroy_image 的图片指针”链表节点
** 字段：
**   ptr ：mlx image 指针（void*）
**   next：下一节点
*/
typedef struct s_img
{
	void *ptr;
	struct s_img *next;
} t_img;

/*
** 结构体：t_coords
** 作用：浮点坐标（玩家位置、射线命中点等）
*/
typedef struct s_coordinates
{
	float x;
	float y;
} t_coords;

/*
** 结构体：t_int_xy
** 作用：整数坐标（地图格子索引、像素坐标等）
*/
typedef struct s_int_coordinates
{
	int x;
	int y;
} t_int_xy;

/* ===================== 纹理 ===================== */
typedef struct s_texture
{
	void *img_ptr;
	char *path;
	char *data;
	int width;
	int height;
	int bpp;
	int size_line;
	int endian;
} t_tex;
/*
** 结构体：t_dda
** 作用：DDA（Digital Differential Analyzer）射线步进算法用的临时状态包
** 字段含义：
**   map_x/map_y : 当前射线所在的地图格子坐标
**   step_x/step_y : 射线在 x/y 方向每次步进是 +1 还是 -1
**   side : 0=撞到x方向的格子边（竖墙），1=撞到y方向的格子边（横墙）
**   delta_x/delta_y : 射线从一条x/y网格线走到下一条网格线需要走的“距离增量”
**   side_x/side_y : 从玩家当前位置到下一条x/y网格线的初始距离（会在循环里不断累加）
**   perp_dist : 命中墙后的“垂直距离”（用于计算墙高，避免鱼眼）
*/
typedef struct s_dda
{
	int map_x;
	int map_y;
	int step_x;
	int step_y;
	int side;
	float delta_x;
	float delta_y;
	float side_x;
	float side_y;
	float perp_dist;
} t_dda;

/* ===================== 玩家 ===================== */
typedef struct s_player
{
	float x;
	float y;
	float angle;

	int map_x;
	int map_y;

	/* 按键状态（必做只需要这些） */
	int key_up;
	int key_down;
	int key_left;
	int key_right;
	int key_rot_l;
	int key_rot_r;

	float move_speed;
	float rot_speed;
	/* 鼠标 */
	int mouse_enabled;
	float mouse_sens;
} t_player;

/* ===================== 内存追踪节点（如果你项目有） ===================== */
typedef struct s_gnode
{
	void *ptr;
	struct s_gnode *next;
} t_gnode;

/* ===================== 配置行类型（parse 用） ===================== */
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
} t_line_type;

/* ===================== Sprite ===================== */

typedef enum e_sprite_type
{
	SPR_TREE,	 // 静态
	SPR_TORCH,	 // 动画
	SPR_MONSTER, // 立体/八方向
	SPR_BARREL,	 // 静态 - 矮胖（需特殊缩放）
	SPR_COUNT	 // 总数
} t_sprite_type;

typedef struct s_sprite
{
	float x;			   /* 地图 X 坐标 */
	float y;			   /* 地图 Y 坐标 */
	float dist;			   /* 到玩家的距离（用于排序） */
	int type;			   /* 精灵类型 (使用你的 ENUM: SPR_TREE, SPR_MONSTER 等) */
	int cur_frame;		   /* 当前播放到第几帧 (用于动画) */
	long long last_update; /* 上次更新帧的时间（或计数器），控制动画频率 */
	float angle;		   /* 精灵自身的面朝方向 (如果是怪物，用于计算 8 方向贴图) */
} t_sprite;

typedef struct s_sprite_config
{
    t_tex   *frames;         // 贴图数组
    int     frame_count;     // 帧数
    bool    is_animated;     // 是否是动画
    bool    is_directional;  // 是否是八方向怪物
    
    /* 🏆 添加下面这三个字段来修复报错 */
    float   h_div;           // 水平缩放比例 (Horizontal Division)
    float   v_div;           // 垂直缩放比例 (Vertical Division)
    float   v_move;          // 垂直偏移 (Vertical Move)
}   t_sprite_config;

typedef struct s_sprite_manager
{
	t_sprite *list;					   /* 动态分配的精灵数组（从地图扫描到的） */
	int num;						   /* 精灵总数 */
	t_sprite_config config[SPR_COUNT]; /* 资源库 */
} t_sprite_manager;

// 在 t_game 里：

/* ===================== 总结构体：t_game ===================== */
typedef struct s_game
{
	/* mlx 资源 */
	void *mlx;
	void *win;

	/* 主画布 */
	void *img;
	char *data;
	int bpp;
	int size_line;
	int endian;

	/* 贴图 */
	t_tex north;
	t_tex south;
	t_tex east;
	t_tex west;

	/* 地板/天花颜色 */
	int floor_color;
	int ceiling_color;

	/* 地图 */
	char **map;
	int map_w;
	int map_h;
	/*
	** 字段：entire_cubfile / cubfile_lines
	** 作用：
	**   entire_cubfile：读取到的完整文件文本（用于 splitlines）
	**   cubfile_lines：按行切开的字符串数组（parse_config/parse_map 都用它）
	*/
	char *entire_cubfile;
	char **cubfile_lines;

	/* 玩家 */
	t_player player;

	/* track_malloc 链表头 */
	t_gnode *track_head;

	t_img *img_head;

	/* 迷你地图比例（如果你用到） */
	float pix_per_unit;
	float focal_length;

	/* 小精灵管理 */
	t_sprite_manager sprs;
	/* 这里的 config 是核心：它存储了每种精灵的贴图“模板” */
	t_sprite_config config[SPR_COUNT];
	float time;
	/* ===== 门（BONUS）===== */

	t_tex door;					/* 关闭门贴图 */
	t_tex door_open;			/* 打开门贴图（新增） */
	unsigned char **door_state; /* 0=关 1=开 */

	/*存储每一列墙到玩家的距离*/
	float z_buffer[WIDTH];

} t_game;

typedef struct s_render_vars
{
	int x;			 /* 当前屏幕列号 */
	int start;		 /* 墙柱绘制起始 y */
	int end;		 /* 墙柱绘制结束 y */
	int line_h;		 /* 墙柱高度 */
	int tex_x;		 /* 贴图 x 坐标 */
	t_tex *tex;		 /* 当前使用的贴图（NO/SO/WE/EA） */
	float perp_dist; // 新增：这列命中物体的距离
} t_render_vars;

/*
** 结构体：t_sprite_render_vars
** 作用：存储精灵投影到屏幕后的临时几何参数
** 字段含义：
** sprite_h / sprite_w : 精灵在屏幕上的像素高度和宽度
** screen_x           : 精灵中心点在屏幕上的 X 坐标（像素位置）
** draw_start_y/end_y : 垂直绘制的像素范围
** draw_start_x/end_x : 水平绘制的像素范围
*/
typedef struct s_sprite_render_vars
{
    int     sprite_h;
    int     sprite_w;
    int     draw_start_y;
    int     draw_end_y;
    int     draw_start_x;
    int     draw_end_x;
    int     screen_x;
    int     v_offset;
    int     type;       // 👈 必须添加这一行
} t_sprite_render_vars;

/* ========== 结构体都定义完了，再引入函数声明 ========== */
#include "func.h"

#endif
