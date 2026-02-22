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

typedef struct s_game				t_game;
typedef struct s_gnode				t_gnode;
typedef struct s_texture			t_tex;
typedef struct s_player				t_player;
typedef struct s_coordinates		t_coords;
typedef struct s_int_coordinates	t_int_xy;
typedef enum e_line_type			t_line_type;
typedef struct s_render_vars		t_render_vars;
typedef struct s_sprite_render_vars	t_sprite_render_vars;
typedef struct s_dda				t_dda;
typedef struct s_ray				t_ray;

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

void								graceful_exit(t_game *game, int exit_code,
										const char *func, const char *msg);
void								print_error(const char *func,
										const char *msg);
void								destroy_images(t_game *game);

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

void								init_game(t_game *game);
void								setup_mlx(t_game *game);
void								setup_hooks(t_game *game);
void								remember_image(t_game *game, void *ptr);
void								load_texture(t_game *game, t_tex *tex);
void								load_wall_textures(t_game *game);

int									game_step(t_game *game);

int									on_window_close(void *param);

int									on_key_down(int keycode, t_game *game);
int									on_key_up(int keycode, t_game *game);

void								update_player(t_game *game);

void								put_pixel(int x, int y, int color,
										t_game *game);

void								clear_image(t_game *game);

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
										t_tex *tex, t_ray ray);

void								draw_wall(t_render_vars v, t_game *game,
										float step, float tex_pos);

void								render_column(t_game *game, float r_dir_x,
										float r_dir_y, int i);

void								draw_loop(t_game *game);

void								render_minimap(t_game *game);

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

void								update_sprite_distances(t_game *game);
void								sort_sprites(t_game *game);
void								render_sprites(t_game *game);
void								init_sprite_texture(t_game *game);

void								load_all_sprite_resources(t_game *game);
void								count_sprites(t_game *game);

#endif