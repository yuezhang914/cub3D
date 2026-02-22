/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 22:33:09 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 22:33:12 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* =========================
 * 结构体：t_img
 * 作用：保存 MLX image 指针的链表节点，用来“统一销毁图片资源”（防泄漏）
 * 在哪调用：
 *   - init.c: remember_image() 把 image 记录到链表
 * 内容物在哪读改：
 *   - 写：remember_image(game, img_ptr)（把 ptr 塞进节点）
 *   - 读：退出/清理函数（遍历链表对 ptr 调 mlx_destroy_image）
 * ========================= */
typedef struct s_img
{
	void					*ptr;   // [写] remember_image() 写入；[读] 清理时用来 mlx_destroy_image
	struct s_img			*next;  // [写] remember_image() 头插链接；[读] 遍历链表释放所有 image
}							t_img;


/* =========================
 * 结构体：t_coords
 * 作用：二维浮点坐标（世界坐标/方向向量都用它）
 * 在哪调用：
 *   - draw_loop.c: ray_dir.x / ray_dir.y（射线方向）
 *   - 其它模块：很多地方用于位置、方向、向量
 * 内容物在哪读改：
 *   - 写：生成射线方向/计算坐标时写
 *   - 读：渲染/碰撞/数学计算时读
 * ========================= */
typedef struct s_coordinates
{
	float					x;     // x 坐标（世界坐标或向量分量）
	float					y;     // y 坐标（世界坐标或向量分量）
}							t_coords;


/* =========================
 * 结构体：t_int_xy
 * 作用：二维整型坐标（格子坐标、像素坐标等“必须是整数”的地方）
 * 在哪调用：
 *   - DDA/地图索引/数组下标时（map_x/map_y 类似场景）
 * 内容物在哪读改：
 *   - 写：把 float 坐标转 int 或计算数组下标时写
 *   - 读：访问 map[y][x]、door_state[y][x] 等时读
 * ========================= */
typedef struct s_int_coordinates
{
	int						x;     // 整数 x（常用于格子列号/数组下标）
	int						y;     // 整数 y（常用于格子行号/数组下标）
}							t_int_xy;


/* =========================
 * 结构体：t_tex
 * 作用：一张贴图（texture）的完整信息：路径、MLX image、像素数据地址、尺寸与像素格式
 * 在哪调用：
 *   - texture.c: load_texture() 负责填充 img_ptr/data/width/height/bpp/size_line/endian
 *   - render_column.c / pixel_draw.c: draw_wall() 读取 data/bpp/size_line/width/height 采样像素
 * 内容物在哪读改：
 *   - 写：load_texture()（加载 xpm 后写入所有字段）
 *   - 读：渲染时按 tex_x/tex_y 从 data 取颜色；销毁时读 img_ptr
 * ========================= */
typedef struct s_texture
{
	void					*img_ptr;    // [写] mlx_xpm_file_to_image 返回；[读] 清理时 mlx_destroy_image
	char					*path;       // [写] 解析 .cub 配置时写（NO/SO/WE/EA 等）；[读] 加载贴图时读
	char					*data;       // [写] mlx_get_data_addr 返回像素数组地址；[读] draw_wall 取像素颜色
	int						width;      // [写] mlx_xpm_file_to_image 回填宽；[读] tex_x 计算/边界保护
	int						height;     // [写] mlx_xpm_file_to_image 回填高；[读] tex_y 计算、step=height/line_h
	int						bpp;        // [写] mlx_get_data_addr 回填每像素位数；[读] 计算一个像素占多少字节(bpp/8)
	int						size_line;  // [写] mlx_get_data_addr 回填一行占字节；[读] data 偏移 tex_y*size_line
	int						endian;     // [写] mlx_get_data_addr 回填字节序；[读] 通常渲染不直接用，格式信息保留
}							t_tex;


/* =========================
 * 结构体：t_dda
 * 作用：DDA（Digital Differential Analyzer）走格子算法的“运行状态”
 *       用于 raycasting：射线在地图格子中一步步走，直到撞墙/门
 * 在哪调用：
 *   - ray_init_data.c: ray_init_dda / ray_set_step_x / ray_set_step_y 初始化这些字段
 *   - raycasting_core2.c: ray_step_once 更新 map_x/map_y/side/side_x/side_y
 *   - raycasting_core.c: ray_run_dda_until_hit 循环直到命中
 * 内容物在哪读改：
 *   - 写：初始化阶段写 delta/step/side；运行阶段每步更新 side_x/side_y/map_x/map_y
 *   - 读：命中后读 side 决定墙面方向；读 side_x/side_y/delta 算 perp_dist
 * ========================= */
typedef struct s_dda
{
	int						map_x;      // [写] init：玩家所在格子x；[写] step：每次跨竖线后 += step_x；[读] map[map_y][map_x]
	int						map_y;      // [写] init：玩家所在格子y；[写] step：每次跨横线后 += step_y；[读] map[map_y][map_x]
	int						step_x;     // [写] init：射线向左(-1)还是向右(+1)；[读] step_once 用它更新 map_x
	int						step_y;     // [写] init：射线向上(-1)还是向下(+1)；[读] step_once 用它更新 map_y
	int						side;       // [写] step_once：0=跨竖线(撞东西墙)，1=跨横线(撞南北墙)；[读] 选贴图/算命中点
	float					delta_x;    // [写] init：跨过一条竖网格线需要增加的射线长度；[读] step_once/算 perp_dist
	float					delta_y;    // [写] init：跨过一条横网格线需要增加的射线长度；[读] step_once/算 perp_dist
	float					side_x;     // [写] init：到第一次竖线的距离；[写] step：每跨竖线 += delta_x；[读] perp_dist=side_x-delta_x
	float					side_y;     // [写] init：到第一次横线的距离；[写] step：每跨横线 += delta_y；[read] perp_dist=side_y-delta_y
	float					perp_dist;  // [写] 有些 bonus overlay 会缓存距离；[读] 用于墙高 HEIGHT/perp_dist
}							t_dda;


/* =========================
 * 结构体：t_player
 * 作用：玩家状态（位置、朝向、按键状态、速度、鼠标设置）
 * 在哪调用：
 *   - input.c: on_key_down/on_key_up 写 key_xxx
 *   - movement.c: update_player 读 key_xxx，写 x/y/angle
 *   - raycasting / draw_loop: 读 angle 生成 dir/plane，读 x/y 作为射线起点
 * 内容物在哪读改：
 *   - x/y/angle：movement 更新，渲染读取
 *   - map_x/map_y：通常是出生点提取或辅助缓存（你项目里有 scan_map/extract_player 会写）
 *   - key_*：input 写，movement 读
 * ========================= */
typedef struct s_player
{
	float					x;              // [写] movement.c: try_move_axis 更新；[读] raycasting 起点、碰撞检测
	float					y;              // [写] movement.c: try_move_axis 更新；[读] raycasting 起点、碰撞检测
	float					angle;          // [写] movement.c: 按左右键旋转更新；[读] draw_loop.c 里 cos/sin 得到 dir

	int						map_x;          // [写] 出生点提取时写（玩家所在格子）；[读] 有些逻辑需要整数格子位置
	int						map_y;          // 同上：整数格子坐标缓存

	int						key_up;         // [写] input.c：按下W=1 松开W=0；[读] movement.c：决定前进
	int						key_down;       // [写] input.c；[读] movement.c：决定后退
	int						key_left;       // [写] input.c；[读] movement.c：决定左平移(A)
	int						key_right;      // [写] input.c；[读] movement.c：决定右平移(D)
	int						key_rot_l;      // [写] input.c；[读] movement.c：决定向左旋转
	int						key_rot_r;      // [写] input.c；[读] movement.c：决定向右旋转

	float					move_speed;     // [写] init_game 设置默认速度；[读] movement.c 用它算 dx/dy
	float					rot_speed;      // [写] init_game 设置默认；[读] movement.c 用它改变 angle

	int						mouse_enabled;  // [写] init_game：bonus=1 base=0；[读] 鼠标回调里判断是否启用
	float					mouse_sens;     // [写] init_game；[读] 鼠标移动改变 angle 时的系数
}							t_player;


/* =========================
 * 结构体：t_gnode
 * 作用：通用“追踪分配内存”的链表节点（你们的 track_malloc 系统）
 * 在哪调用：
 *   - track_malloc/track_head（你项目内存管理模块）会创建/遍历它
 * 内容物在哪读改：
 *   - ptr：保存 malloc 得到的地址
 *   - next：链表链接
 * ========================= */
typedef struct s_gnode
{
	void					*ptr;   // [写] track_malloc 新分配时记录；[读] 统一 free 时遍历释放
	struct s_gnode			*next;  // [写] 头插/尾插；[读] 遍历链表
}							t_gnode;


/* =========================
 * 结构体：t_sprite
 * 作用：一个 sprite 实体（物体/敌人/火把等）的运行数据：位置、距离、类型、动画帧、朝向
 * 在哪调用：
 *   - bonus sprite 系统：scan/生成 sprite 列表；渲染前 sort_sprites；render_sprites 读取这些字段
 * 内容物在哪读改：
 *   - x/y：创建 sprite 时写；渲染时读
 *   - dist：每帧计算玩家到 sprite 距离写；排序时读
 *   - cur_frame/last_update：动画更新写；绘制读
 * ========================= */
typedef struct s_sprite
{
	float					x;           // [写] sprite 初始化/生成；[读] 渲染时计算屏幕位置
	float					y;           // 同上
	float					dist;        // [写] 每帧更新距离；[读] sort_sprites 用来排序
	int						type;        // [写] 生成时标记类型（SPR_TREE 等）；[读] 选用哪组贴图配置
	int						cur_frame;   // [写] 动画更新切帧；[读] 取 frames[cur_frame] 贴图
	long long				last_update; // [写] 动画时间戳；[读] 判断是否到下一帧
	float					angle;       // [写/读] directional sprite 可能用；决定朝向贴图选择
}							t_sprite;


/* =========================
 * 结构体：t_sprite_config
 * 作用：某一类 sprite 的“配置”（贴图帧数组、帧数、是否动画/方向性、缩放与偏移）
 * 在哪调用：
 *   - init.c (bonus) init_sprite_params() 设置默认 h_div/v_div/v_move
 *   - sprite 贴图加载模块填 frames/frame_count/is_animated 等
 * 内容物在哪读改：
 *   - frames/frame_count：贴图加载时写；渲染读
 *   - is_animated/is_directional：加载/配置时写；渲染时决定取哪一帧/哪一方向
 *   - h_div/v_div/v_move：初始化写；渲染计算 sprite 高宽和上下偏移时读
 * ========================= */
typedef struct s_sprite_config
{
	t_tex					*frames;         // [写] 加载 sprite 贴图后指向帧数组；[读] 渲染时 frames[cur_frame]
	int						frame_count;     // [写] 加载完成后写；[读] 动画循环边界
	bool					is_animated;     // [写] 配置写；[读] 是否需要根据时间更新 cur_frame
	bool					is_directional;  // [写] 配置写；[读] 是否需要根据 sprite.angle/玩家方向选贴图

	float					h_div;           // [写] init_sprite_params 默认1；[读] 控制 sprite 在屏幕上的宽度缩放
	float					v_div;           // [写] init_sprite_params 默认1；[读] 控制 sprite 高度缩放
	float					v_move;          // [写] init_sprite_params 对部分类型调整；[读] 控制 sprite 上下移动
}							t_sprite_config;


/* =========================
 * 结构体：t_sprite_manager
 * 作用：管理全部 sprite 实体 + 每种 sprite 的配置表
 * 在哪调用：
 *   - loop.c bonus: handle_bonus_rendering() 里 sort_sprites/render_sprites 会用到
 * 内容物在哪读改：
 *   - list/num：生成 sprite 时写；每帧渲染读；可能动态增删也会写
 *   - config[]：初始化/加载时写；渲染时按 type 读取配置
 * ========================= */
typedef struct s_sprite_manager
{
	t_sprite				*list;                 // [写] 生成 sprite 列表；[读] 渲染遍历
	int						num;                  // [写] 生成数量；[读] loop.c 判断是否需要渲染 sprite
	t_sprite_config			config[SPR_COUNT];    // [写] 初始化/加载；[读] 渲染取对应类型配置
}							t_sprite_manager;


/* =========================
 * 结构体：t_game（项目核心总结构体）
 * 作用：把“整个游戏运行所需的一切状态”都集中在一起：mlx、画布、贴图、地图、玩家、门、sprite、z_buffer 等
 *
 * 在哪调用：
 *   - 几乎所有模块函数的第一个参数都是 t_game* game
 *   - render：读 data/bpp/size_line、读贴图、读颜色
 *   - input/movement：读写 player
 *   - parse：写入贴图路径、颜色、map、door_state 等
 *   - raycasting：读 player/map/door_state，写 z_buffer
 *
 * 内容物在哪读改（按类别总结）：
 *   - MLX/画布：setup.c 写，pixel_draw.c/clear_image 写像素，loop.c mlx_put_image_to_window 读
 *   - 贴图：parse 写 path，texture.c 写 img_ptr/data/width/height...
 *   - 地图/门：parse 写 map/door_state，movement.c 的 tile_blocks 读
 *   - player：input 写 key，movement 写 x/y/angle，draw_loop/raycasting 读
 *   - 内存追踪：track_malloc 写 track_head；退出时遍历释放
 * ========================= */
typedef struct s_game
{
	/* ===== MLX 运行环境与窗口 ===== */
	void					*mlx;          // [写] setup_mlx: mlx_init；[读] 创建窗口/图片/put_image 时用
	void					*win;          // [写] setup_mlx: mlx_new_window；[读] mlx_put_image_to_window 时用

	/* ===== 离屏画布（每帧先画到 img，再贴到窗口） ===== */
	void					*img;          // [写] setup_mlx: mlx_new_image；[读] mlx_get_data_addr/put_image_to_window；[清理] destroy
	char					*data;         // [写] setup_mlx: mlx_get_data_addr；[写] put_pixel/draw_wall 往里写像素
	int						bpp;          // [写] mlx_get_data_addr 回填；[读] put_pixel/draw_wall 计算像素字节
	int						size_line;    // [写] mlx_get_data_addr 回填；[读] 计算 data 偏移
	int						endian;       // [写] mlx_get_data_addr 回填；[读] 通常不直接用

	/* ===== 墙贴图（四个方向） ===== */
	t_tex					north;        // [写] parse_config 写 path；[写] load_texture 填 img_ptr/data；[读] ray_pick_texture 选用
	t_tex					south;        // 同上
	t_tex					east;         // 同上
	t_tex					west;         // 同上

	/* ===== 天花板/地板颜色 ===== */
	int						floor_color;   // [写] parse_config set_color_config；[读] render_column 画地板
	int						ceiling_color; // [写] parse_config；[读] render_column 画天花板

	/* ===== 地图（字符二维数组） ===== */
	char					**map;         // [写] parse_map build_map_array；[读] movement tile_blocks/raycasting 等
	int						map_w;        // [写] parse_utils set_map_dimensions；[读] 越界判断/渲染
	int						map_h;        // [写] parse_utils set_map_dimensions；[read] 越界判断/渲染

	/* ===== .cub 原始内容（调试/解析用） ===== */
	char					*entire_cubfile;  // [写] parse_entry import_cub(ft_readfile)；[读] 一般不再用，仅保留
	char					**cubfile_lines;  // [写] splitlines；[读] parse_config/parse_map 逐行分析

	/* ===== 玩家 ===== */
	t_player				player;       // [写] scan_map/extract_player 初始位置；[写] movement 每帧更新；[读] raycasting/render

	/* ===== 追踪 malloc 的链表（统一 free） ===== */
	t_gnode					*track_head;  // [写] track_malloc 每次分配都挂链表；[读] graceful_exit 清理

	/* ===== 追踪 MLX image 的链表（统一 destroy_image） ===== */
	t_img					*img_head;    // [写] remember_image；[读] graceful_exit 清理 image

	/* ===== 渲染常量/透视参数 ===== */
	float					pix_per_unit;   // [写] init_game 设置；[读] minimap/sprite 等可能用（比例尺）
	float					focal_length;   // [写] init_game 根据 FOV 计算；[读] draw_loop setup_camera 计算 plane

	/* ===== sprite 系统（BONUS） ===== */
	t_sprite_manager		sprs;          // [写] bonus 初始化/生成；[读] loop.c bonus 渲染
	t_sprite_config			config[SPR_COUNT]; // [写] init_sprite_params 默认值；[读] sprite 渲染用（注意和 sprs.config 重复字段）

	/* ===== 时间（用于动画/门/帧同步等） ===== */
	float					time;          // [写] loop.c 每帧 time+=0.016；[读] 动画/门逻辑可能用

	/* ===== 门贴图与门状态（BONUS） ===== */
	t_tex					door;          // [写] door 贴图加载；[读] ray_pick_texture 选择关门贴图
	t_tex					door_open;     // [写] door_open 贴图加载；[读] 开门 overlay/开门贴图
	unsigned char			**door_state;  // [写] bonus 初始化/解析/try_toggle_door 改状态；[读] movement tile_blocks 判断能否通过

	/* ===== 深度缓冲（每列最近墙距离，用于遮挡） ===== */
	float					z_buffer[WIDTH]; // [写] raycasting_cal get_render_vars 设置 z_buffer[i]=dist；[读] sprite/door overlay 判断遮挡

}							t_game;


/* =========================
 * 结构体：t_render_vars
 * 作用：渲染“屏幕某一列 x”的参数包（墙柱高度、起止y、贴图、贴图列、距离）
 * 在哪调用：
 *   - get_render_vars() 返回它（raycasting 模块）
 *   - render_column.c 使用它画天花板/墙/地板（draw_wall 用到 tex/tex_x/start/end/line_h）
 * 内容物在哪读改：
 *   - 写：raycasting 计算并填充
 *   - 读：render_column / pixel_draw 读取
 * ========================= */
typedef struct s_render_vars
{
	int						x;         // [写] get_render_vars 设置为屏幕列号；[读] put_pixel 用它当 x 坐标
	int						start;     // [写] raycasting 计算墙柱开始 y；[读] render_column 画天花板/墙
	int						end;       // [写] raycasting 计算墙柱结束 y；[读] render_column 画墙/地板
	int						line_h;    // [写] = HEIGHT / perp_dist；[读] step = tex_h / line_h
	int						tex_x;     // [写] ray_calc_tex_x 算出；[读] draw_wall 从贴图取这一列像素
	t_tex					*tex;      // [写] ray_pick_texture 选择；[read] draw_wall 读取 data/height/size_line/bpp
	float					perp_dist; // [写] ray_get_perp_dist；[读] z_buffer/墙高/遮挡
}							t_render_vars;


/* =========================
 * 结构体：t_sprite_render_vars
 * 作用：sprite 渲染时的“屏幕投影参数包”（计算出 sprite 在屏幕的矩形范围）
 * 在哪调用：
 *   - sprite 渲染模块：先算这些值，再按范围画 sprite 像素
 * 内容物在哪读改：
 *   - 写：sprite 投影计算函数（比如 calc_sprite_vars）
 *   - 读：真正画 sprite 像素的循环
 * ========================= */
typedef struct s_sprite_render_vars
{
	int						sprite_h;      // sprite 在屏幕上的高度（像素）
	int						sprite_w;      // sprite 在屏幕上的宽度（像素）
	int						draw_start_y;  // 屏幕上 y 起点（裁剪后）
	int						draw_end_y;    // 屏幕上 y 终点（裁剪后）
	int						draw_start_x;  // 屏幕上 x 起点
	int						draw_end_x;    // 屏幕上 x 终点
	int						screen_x;      // sprite 中心投影到屏幕的 x（用于左右范围）
	int						v_offset;      // 垂直偏移（配合 v_move）
	int						type;          // sprite 类型（对应 SPR_*）
}							t_sprite_render_vars;


/* =========================
 * 结构体：t_draw_ctx
 * 作用：sprite 绘制时的“上下文”：把 game + 当前 sprite 的投影参数 + 当前选用贴图 + 循环边界放一起
 * 在哪调用：
 *   - sprite 绘制函数：传一个 ctx，里面所有参数都齐，不用传一堆散参数
 * 内容物在哪读改：
 *   - 写：sprite 渲染准备阶段填充
 *   - 读：sprite 绘制循环中不断读取 sx/ex/sy/ey、tex、trans_y 等
 * ========================= */
typedef struct s_draw_ctx
{
	t_game					*game;   // [写] 创建 ctx 时赋值；[读] 画 sprite 时需要 data/z_buffer/贴图/玩家等
	t_sprite_render_vars	v;      // [写] 投影计算后填；[读] 绘制循环使用范围

	float					trans_y; // [写] sprite 相对玩家变换后的 y（深度）；[读] 用于缩放和遮挡
	t_tex					*tex;    // [写] 根据 sprite 类型/帧选择贴图；[读] 取像素颜色

	int						sx;      // [写] draw_start_x；[读] for/while 画 x 的起点
	int						ex;      // [写] draw_end_x；[读] 画 x 的终点

	int						sy;      // [写] draw_start_y；[read] 画 y 起点
	int						ey;      // [write] draw_end_y；[read] 画 y 终点

}							t_draw_ctx;


/* =========================
 * 结构体：t_ray
 * 作用：保存一条射线的关键数值（方向 + 距离），方便纹理计算时打包传参
 * 在哪调用：
 *   - raycasting_core.c / raycasting_core2.c: ray_calc_tex_x 会用 dir_x/dir_y/perp_dist
 * 内容物在哪读改：
 *   - 写：ray_make_render_vars 组装
 *   - 读：ray_calc_tex_x 读取
 * ========================= */
typedef struct s_ray
{
	float					dir_x;      // 射线方向 x 分量（由 draw_loop 的 ray_dir.x 或类似逻辑得到）
	float					dir_y;      // 射线方向 y 分量
	float					perp_dist;  // 命中距离（用于计算 hit 点与墙高）
}							t_ray;
