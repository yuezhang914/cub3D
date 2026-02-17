/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:36:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/17 18:57:07 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx-linux/mlx.h"
# include "func.h"

# define W 119
# define A 97
# define S 115 // south
# define D 100
# define ESC 65307
# define PI 3.14159265359

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define LEFT 65361
# define RIGHT 65363
# define DEBUG 0
# define BUFFER_SIZE 1024
# define FOV 66.0
# define MINI_HEIGHT 200
# define MINI_WIDTH 200
# define DISTANCE_SEEN 5
/* 玩家碰撞缓冲：离墙太近时不允许继续前进（单位：格子坐标） */
# define WALL_BUFFER 0.20f

/* ========================================================================== */
/*  合作分工标记说明（非常重要，避免两个人同时改同一字段）：                    */
/*  [YZ] 由你写（parse + clean + A），队友只读                                  */
/*  [WY] 由队友写（B：movement + raycasting），你只读                           */
/*  [ONCE-YZ] 只在启动/初始化阶段由 YZ 写一次，运行中基本不再改                 */
/*  [RUNTIME-WY] 运行时每帧可能变化，由 WY 写                                   */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/* 浮点坐标：用于“连续空间”坐标（能平滑移动）                                   */
/* 例：玩家位置、射线命中点等。                                                 */
/* 初始值由 parse 写一次（YZ），之后移动时由 WY 每帧更新。                       */
/* -------------------------------------------------------------------------- */
typedef struct s_coordinates
{
	float x; /* x 坐标（向右增大） [ONCE-YZ 初始][RUNTIME-WY 更新][YZ 读] */
	float y; /* y 坐标（向下增大） [ONCE-YZ 初始][RUNTIME-WY 更新][YZ 读] */
}				t_coords;

/* -------------------------------------------------------------------------- */
/* 整数坐标：用于“格子坐标/像素坐标”                                             */
/* 例：map[y][x] 的索引、或屏幕像素点坐标。                                      */
/* 通常是函数内部临时变量，谁用谁算（不会在 t_game 里长期共享）。                */
/* -------------------------------------------------------------------------- */
typedef struct s_int_coordinates
{
	int x; /* 整数 x（列号/像素x） */
	int y; /* 整数 y（行号/像素y） */
}				t_int_xy;

/* -------------------------------------------------------------------------- */
/* parse 用：给 .cub 文件每一行分类                                                */
/* 例如：NO/SO/WE/EA/F/C/地图行/空行/非法行                                       */
/* -------------------------------------------------------------------------- */
typedef enum e_line_type
{
	EMPTY,
	NORTH,
	EAST,
	SOUTH,
	WEST,
	FLOOR,
	CEILING,
	MAP,
	WRONG,
}				t_line_type;

/* -------------------------------------------------------------------------- */
/* 退出时销毁 MLX image 的链表节点：单链表                                        */
/* YZ 在创建 image 或加载 xpm 时把 img_ptr 记下来，退出统一销毁。                   */
/* -------------------------------------------------------------------------- */
typedef struct s_img
{
	void *ptr;          /* mlx image 指针（mlx_new_image
        / mlx_xpm...）[ONCE-YZ 写] */
	struct s_img *next; /* 单链表 next [ONCE-YZ 写] */
}				t_img;

/* -------------------------------------------------------------------------- */
/* 贴图（纹理）结构：一张 xpm 图加载进 MLX 之后的完整信息                          */
/* 重点理解：parse 只负责写 path；setup 负责真正加载到内存并填充其余字段；         */
/* render 使用 data/bpp/size_line 等去取像素颜色。                                */
/* -------------------------------------------------------------------------- */
typedef struct s_texture
{
	char *path;    /* xpm 文件路径 [ONCE-YZ:parse 写][WY 不写] */
	void *img_ptr; /* mlx 图像对象 [ONCE-YZ:setup 写] */
	int *addr;     /* 像素数据的整数型首地址 (可理解为 data 的 int 映射) [YZ:render 读] */
	int width;     /* 贴图宽度 [ONCE-YZ:setup 写] */
	int height;    /* 贴图高度 [ONCE-YZ:setup 写] */
	void *data;    /* 像素数组首地址 [ONCE-YZ:setup 写][YZ:render 读] */
	int bpp;       /* 每像素位数 bits-per-pixel [ONCE-YZ:setup 写] */
	int size_line; /* 每行字节数（stride）[ONCE-YZ:setup 写] */
	int endian;    /* 字节序 [ONCE-YZ:setup 写] */
}				t_tex;
/* -------------------------------------------------------------------------- */
/* 简易 GC（垃圾回收）链表节点：记录 malloc 的指针，退出统一 free                  */
/* 建议：只有 YZ 操作链表结构；WY 若要分配内存，只调用 f_track_malloc。               */
/* -------------------------------------------------------------------------- */
typedef struct s_gnode
{
	void *ptr;            /* malloc 返回的指针 [YZ 维护] */
	struct s_gnode *next; /* next [YZ 维护] */
}				t_gnode;

typedef struct s_render_vars
{
	int x;      /* 当前渲染的屏幕像素列索引 (0 到 WIDTH - 1) */
	int start;  /* 墙体在屏幕上绘制的起始 Y 坐标 */
	int end;    /* 墙体在屏幕上绘制的结束 Y 坐标 */
	int line_h; /* 墙体在垂直方向上的原始像素高度 */
	int tex_x;  /* 对应纹理贴图的 X 坐标 (采样位置) */
	t_tex *tex; /* 当前射线击中的墙面所对应的纹理指针 */
}				t_render_vars;

typedef struct s_player
{
	float x;            /* 玩家在地图上的精确 X 坐标 (浮点数，用于移动) */
	float y;            /* 玩家在地图上的精确 Y 坐标 (浮点数，用于移动) */
	int map_x;          /* 玩家当前所在的整数格子 X 坐标 (用于 DDA 初始检查) */
	int map_y;          /* 玩家当前所在的整数格子 Y 坐标 (用于 DDA 初始检查) */
	float angle;        /* 玩家当前的视线角度 (弧度制) */
	bool key_up;        /* W 键状态：是否正在向前移动 */
	bool key_down;      /* S 键状态：是否正在向后移动 */
	bool key_left;      /* A 键状态：是否正在向左平移 (Strafe) */
	bool key_right;     /* D 键状态：是否正在向右平移 (Strafe) */
	bool left_rotate;   /* 左方向键状态：是否正在向左旋转视角 */
	bool right_rotate;  /* 右方向键状态：是否正在向右旋转视角 */
	float move_speed;   /* 玩家移动步长 (帧间隔移动距离) */
	float rotate_speed; /* 玩家旋转速度 (帧间隔旋转弧度) */
}				t_player;

/*
** DDA (Digital Differential Analyzer) 算法中间变量结构体
** 用于射线投射过程中的步进计算和碰撞检测
*/
typedef struct s_dda
{
	int map_x;       // 射线当前所在的地图格子 X 坐标
	int map_y;       // 射线当前所在的地图格子 Y 坐标
	int step_x;      // X 轴步进方向 (1 或 -1)
	int step_y;      // Y 轴步进方向 (1 或 -1)
	float side_x;    // 起点到下一个 X 边界的初始/累加距离
	float side_y;    // 起点到下一个 Y 边界的初始/累加距离
	float delta_x;   // 射线从一个 X 边界移动到下一个 X 边界所需的距离
	float delta_y;   // 射线从一个 Y 边界移动到下一个 Y 边界所需的距离
	float perp_dist; // 射线起点到墙面的垂直距离 (用于修正鱼眼)
	int side;        // 碰撞性质：0 代表东西墙 (左右)，1 代表南北墙 (上下)
}				t_dda;

/* -------------------------------------------------------------------------- */
/* 主结构体：全局“数据总线”（所有模块都通过它传递信息）                            */
/* 重要：字段写入者分工要固定，否则两人同时改会冲突。                               */
/* -------------------------------------------------------------------------- */
typedef struct s_game
{
	/* ====== mandatory 四向墙贴图 ====== */
	t_tex north; /* [YZ] parse 写 path；setup 写 img/data；render 读 */
	t_tex east;  /* 同上 */
	t_tex south; /* 同上 */
	t_tex west;  /* 同上 */

	t_img		*img_head;

	/* ====== GC：统一释放 malloc ====== */
	t_gnode *track_head; /* [YZ] 维护链表头，退出统一 free */

	/* ====== 配置/解析结果：颜色 ====== */
	int floor_color;   /* 地板颜色 [ONCE-YZ parse 写][YZ render 读] */
	int ceiling_color; /* 天花板颜色 [ONCE-YZ parse 写][YZ render 读] */

	/* ====== 读入的 .cub 文件内容（parse 用） ====== */
	char *entire_cubfile; /* 整个文件字符串 [ONCE-YZ parse 写] */
	char **cubfile_lines; /* 按行切分数组 [ONCE-YZ parse 写] */
	int cubfile_fd;       /* open 后的 fd [ONCE-YZ parse 写] */

	/* ====== 地图信息（parse 生产，movement+raycast+render 消费） ====== */
	int map_h;  /* 地图高度（行数）[ONCE-YZ parse 写][WY 读][YZ 读] */
	int map_w;  /* 地图最大宽度 [ONCE-YZ parse 写][WY 读][YZ 读] */
	char **map; /* 二维地图 map[y][x] [ONCE-YZ parse 写][WY 读][YZ 读] */

	/* ====== MLX 对象（窗口/图像） ====== */
	void *mlx; /* mlx_init 返回 [ONCE-YZ setup 写] */
	void *win; /* mlx_new_window 返回 [ONCE-YZ setup 写] */
	void *img; /* mlx_new_image 返回 [ONCE-YZ setup 写] */

	/* ====== 屏幕帧缓冲（把像素写到这里，最后 put 到窗口） ====== */
	char *data;    /* 图像像素数组首地址 [ONCE-YZ setup 写][YZ render 写像素] */
	int bpp;       /* 每像素位数 [ONCE-YZ setup 写][YZ render 读] */
	int size_line; /* 每行字节数 [ONCE-YZ setup 写][YZ render 读] */
	int endian;    /* 字节序 [ONCE-YZ setup 写][YZ render 读] */

	/* ====== 渲染参数 ====== */
	int pix_per_unit;   /* 缩放参数（常用于小地图/比例）[YZ 写] */
	float focal_length; /* 投影焦距（影响墙高）[YZ 写] */

	/* ====== 输入状态：按键是否按下（hook 写，loop 读） ====== */

	t_player	player;

}				t_game;

#endif
