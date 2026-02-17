/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 01:59:13 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/13 21:47:52 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNC_H
# define FUNC_H

// For va_list, va_start, va_end
# include <stdarg.h>
// For open(), close()
# include <fcntl.h>
// For read(), write()
# include <unistd.h>
// For printf()
# include <stdio.h>
// For track_malloc(game, ), free()
# include <stdlib.h>
// For perror(), strerror()
# include <string.h>
// For exit()
# include <stdlib.h>
// For gettimeofday()
# include <sys/time.h>
// For booleans
# include <stdbool.h>
// For sin and cos
# include <math.h>
// For keycodes
# include <X11/keysym.h>
// For gettimeofday()
# include <sys/time.h>
# include <stddef.h>

typedef struct s_game	t_game;
typedef struct s_gnode	t_gnode;
typedef struct s_texture	t_tex;
typedef enum e_line_type	t_line_type;
typedef struct s_coordinates	t_coords;
typedef struct s_int_coordinates	t_int_xy;
typedef struct s_img		t_img;
typedef struct s_player  t_player;
typedef struct s_render_vars	t_render_vars;

// utils
size_t	ft_strlen(const char *s);
int	ft_strcmp(char *str1, char *str2);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(t_game *game, const char *s);
char	*ft_strjoin(t_game *game, char const *s1, char const *s2);
int	char_count(char *str, char c);
bool	is_dig(char c);
char	*ft_n_strcmp(char *str1, int n, ...);
void	*ft_memcpy(void *dest, const void *src, size_t n);
t_gnode	*track_add_node(t_game *game, void *ptr);
void	*track_malloc(t_game *game, size_t size);
char	**splitlines(t_game *game, char *text);
char	**ft_split(t_game *game, char const *s, char c);
void	ft_bzero(void *s, size_t n);
char	*ft_readfile(t_game *game, int fd);

// cleanup
void	destroy_images(t_game *game);
void	track_clean(t_game *game);
void	graceful_exit(t_game *game, int exit_code, const char *func,
		char *message);
void	print_error(const char *func, char *message);

// parse
char	*skip_spaces(char *s);
void		check_map_is_last(t_game *game, char **lines, int start);
int			find_map_start(t_game *game, char **lines);
void		set_map_dimensions(t_game *game, char **lines, int start);
void		module_parse(t_game *game, int argc, char **argv);
void		parse_config(t_game *game);
void		parse_map(t_game *game);
t_line_type	set_input_line_type(t_game *game, char *line);

// setup
void	init_game(t_game *game);
void	remember_image(t_game *game, void *ptr);
void	setup_hooks(t_game *game);
void	setup_mlx(t_game *game);
void	load_texture(t_game *game, t_tex *tex);
void	load_wall_textures(t_game *game);

// movement & raycasting
void init_player(t_player *player);
int key_press(int keycode, void *param);
int key_release(int keycode, void *param);
void move_player(t_player *player, t_game *game);
t_render_vars	get_render_vars(t_game *game, float r_dir_x, float r_dir_y, int i);

// render
void render_minimap(t_game *game);
void put_pixel(int x, int y, int color, t_game *game);
void render_column(t_game *game, float r_dir_x, float r_dir_y, int i);
int draw_loop(t_game *game);
//void	load_anim(t_game *game, t_anim *anim, t_anim params, char *path);
// // game
// bool    is_wall(t_game *game, t_coords coords);

// int		game_step(t_game *game);
// int		on_window_close(void *param);

// int		on_key_down(int keycode, t_game *game);
// int		on_key_up(int keycode, t_game *game);
// int		on_mouse_move(int x, int y, void *param);

// void	update_player(t_game *game);
// void	try_move_axis(t_game *game, char axis, float delta);

// t_dir	hit_direction(t_game *game, t_coords wall_hit);
// t_int_xy	hit_tile_coords(t_game *game, t_coords hit, t_dir dir);
// char	hit_tile_type(t_game *game, t_coords hit, t_dir dir);
// t_int_xy	adjacent_tile_ahead(t_game *game);
// t_int_xy	find_tile(t_game *game, char c);

// render
// void put_pixel(int x,int y,int color,t_game *game);
// void clear_image(t_game *game);
// int tex_sample(t_tex *tex, t_coords uv);
// int wall_tex_color(t_game *game, t_coords wall_hit, float rel_y);
// void draw_walls(t_game *game);
// void draw_image(t_game *game, t_tex *tex, t_int_xy top_left);
// void draw_from_tex_to_area(t_game *game, t_tex *tex, t_rect area);
// void draw_line(t_game *game, t_line line);
// void draw_square(t_game *game, t_square sq);
// void draw_full_square(t_game *game, t_square sq);
// void draw_circle_full(t_game *game, t_circle circle);
// void superpose_anim(t_game *game, t_int_xy tile, t_coords relative, int *color);


#endif