/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:59:13 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/19 10:47:24 by weiyang        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNC_H
# define FUNC_H

/* ========== 系统库 ========== */
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

/* ========== 结构体/类型前置声明（避免互相 include 死循环） ========== */
typedef struct s_game				t_game;
typedef struct s_gnode				t_gnode;
typedef struct s_texture			t_tex;
typedef struct s_player				t_player;
typedef struct s_coordinates		t_coords;
typedef struct s_int_coordinates	t_int_xy;
typedef enum e_line_type			t_line_type;
typedef struct s_render_vars		t_render_vars;
typedef struct s_sprite_render_vars	t_sprite_render_vars;

/*
** 结构体：t_render_vars
** 作用：DDA/列渲染时，每一列需要的一揽子参数（起止y、贴图、tex_x、列号等）
** 说明：这个类型会被 get_render_vars() “按值返回”，所以必须在 func.h 里完整定义
**      （只写 typedef 不够，会导致 unknown type / incomplete type）
*/

/* ===================== utils（你项目里的） ===================== */
size_t								ft_strlen(const char *s);
int									ft_strcmp(char *s1, char *s2);
char								*ft_strchr(const char *s, int c);
void								ft_bzero(void *s, size_t n);
void								*ft_memcpy(void *dst, const void *src,
										size_t n);
char								*ft_strdup(t_game *game, const char *s);
char								*ft_strjoin(t_game *game, char const *s1,
										char const *s2);
char								*ft_readfile(t_game *game, int fd);
char								**ft_split(t_game *game, char const *s,
										char c);
char								**splitlines(t_game *game, char *text);
bool								is_dig(char c);
int									char_count(char *str, char c);
char								*ft_n_strcmp(char *str1, int n, ...);

t_gnode								*track_add_node(t_game *game, void *ptr);
void								*track_malloc(t_game *game, size_t size);

/* ===================== cleanup ===================== */
/*
** 函数：graceful_exit
** 作用：统一释放资源并退出；exit_code!=0 时打印 Error 信息
** 参数：
**   game：总结构体
**   exit_code：退出码（0=正常，1=错误）
**   func：出错函数名（可为 NULL）
**   msg：错误信息（可为 NULL）
*/
void								graceful_exit(t_game *game, int exit_code,
										const char *func, const char *msg);
void								print_error(const char *func,
										const char *msg);
void								destroy_images(t_game *game);

/* ===================== parse ===================== */
void								module_parse(t_game *game, int argc,
										char **argv);
void								parse_config(t_game *game);
void								scan_map(t_game *game);
void								parse_map(t_game *game);
t_line_type							set_input_line_type(t_game *game,
										char *line);
void								set_map_dimensions(t_game *game,
										char **lines, int start);
int									find_map_start(t_game *game, char **lines);
void								check_map_is_last(t_game *game,
										char **lines, int start);

/* ===================== setup ===================== */
void								init_game(t_game *game);
void								setup_mlx(t_game *game);
void								setup_hooks(t_game *game);
void								remember_image(t_game *game, void *ptr);
void								load_texture(t_game *game, t_tex *tex);
void								load_wall_textures(t_game *game);

/* ===================== input / loop（必做） ===================== */
/*
** 函数：game_step
** 作用：每帧执行：更新玩家→清屏→画墙→贴到窗口（小地图如果你有就再画）
*/
int									game_step(t_game *game);

/*
** 函数：on_window_close
** 作用：点窗口关闭按钮时退出
*/
int									on_window_close(void *param);

/*
** 函数：on_key_down / on_key_up
** 作用：记录按键状态（不要直接移动），移动在每帧 update_player 做
*/
int									on_key_down(int keycode, t_game *game);
int									on_key_up(int keycode, t_game *game);

/* ===================== movement（必做） ===================== */
void								update_player(t_game *game);

/* ===================== render（必做） ===================== */
/*
** 函数：put_pixel
** 作用：往主画布写一个像素
*/
void								put_pixel(int x, int y, int color,
										t_game *game);

/*
** 函数：clear_image
** 作用：清空一帧画布
*/
void								clear_image(t_game *game);

/*
** 函数：get_render_vars
** 作用：计算屏幕第 i 列射线命中信息，并返回列渲染需要的参数包
*/
t_render_vars						get_render_vars(t_game *game, float r_dir_x,
										float r_dir_y, int i);

void								ray_init_dda(t_game *game, t_dda *d,
										float r_dir_x, float r_dir_y);
void								ray_set_step_x(t_game *game, t_dda *d,
										float r_dir_x);
void								ray_set_step_y(t_game *game, t_dda *d,
										float r_dir_y);
int									ray_run_dda_until_hit(t_game *game,
										t_dda *d, float r_dir_x, float r_dir_y);
t_render_vars						ray_make_render_vars(t_game *game, t_dda *d,
										float r_dir_x, float r_dir_y);

void								ray_step_once(t_dda *d);
int									ray_outside_map(t_game *game, t_dda *d);
t_tex								*ray_pick_texture(t_game *game, t_dda *d,
										float r_dx, float r_dy);
float								ray_get_perp_dist(t_dda *d);
int									ray_calc_tex_x(t_game *game, t_dda *d,
										t_tex *tex, float perp, float r_dx,
										float r_dy);

void								draw_wall(t_render_vars v, t_game *game,
										float step, float tex_pos);

/*
 ** 函数：render_column
 ** 作用：渲染屏幕第 i 列（天花→墙贴图→地板）
 */
void								render_column(t_game *game, float r_dir_x,
										float r_dir_y, int i);

/*
** 函数：draw_walls
** 作用：遍历所有列，调用 render_column 画出 3D 墙
*/
void								draw_loop(t_game *game);

/* 小地图 */
void								render_minimap(t_game *game);

/* Bonus*/
# ifdef BONUS
int									on_mouse_move(int x, int y, t_game *game);
void								enable_mouse(t_game *game);

void								init_doors(t_game *game);
void								load_door_texture(t_game *game);
void								try_toggle_door(t_game *game);
int									door_ray_blocks(t_game *game, t_dda *d,
										float rdx, float rdy);
int									door_is_closed(t_game *game, int x, int y);

t_render_vars						get_open_door_overlay_vars(t_game *game,
										float r_dir_x, float r_dir_y, int i);
void								draw_door_overlay_column_bonus(t_game *game,
										float r_dir_x, float r_dir_y, int i);

bool								check_bonus_elements(t_game *game);
void								collect_sprites(t_game *game);
void								handle_bonus_setup(t_game *game);

void								calculate_sprite_distance(t_game *game);
void								sort_sprites(t_game *game);
void								render_sprites(t_game *game);
void								init_sprite_texture(t_game *game);
# endif

#endif
