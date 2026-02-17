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

# include <stdarg.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdbool.h>
# include <math.h>
# include <X11/keysym.h>
# include <stddef.h>

typedef struct s_game				t_game;
typedef struct s_gnode				t_gnode;
typedef struct s_texture			t_tex;
typedef enum e_line_type			t_line_type;
typedef struct s_coordinates		t_coords;
typedef struct s_int_coordinates	t_int_xy;
typedef struct s_img				t_img;
typedef struct s_player				t_player;
typedef struct s_render_vars		t_render_vars;
typedef struct s_dda				t_dda;

/*
** Libft & Utils
*/
size_t			ft_strlen(const char *s);
int				ft_strcmp(char *str1, char *str2);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(t_game *game, const char *s);
char			*ft_strjoin(t_game *game, char const *s1, char const *s2);
int				char_count(char *str, char c);
bool			is_dig(char c);
char			*ft_n_strcmp(char *str1, int n, ...);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			ft_bzero(void *s, size_t n);
char			**splitlines(t_game *game, char *text);
char			**ft_split(t_game *game, char const *s, char c);
char			*ft_readfile(t_game *game, int fd);

/*
** Memory Tracking & Error Handling
*/
t_gnode			*track_add_node(t_game *game, void *ptr);
void			*track_malloc(t_game *game, size_t size);
void			destroy_images(t_game *game);
void			track_clean(t_game *game);
void			graceful_exit(t_game *game, int exit_code, const char *func, \
				char *message);
void			print_error(const char *func, char *message);

/*
** Parsing
*/
char			*skip_spaces(char *s);
void			check_map_is_last(t_game *game, char **lines, int start);
int				find_map_start(t_game *game, char **lines);
void			set_map_dimensions(t_game *game, char **lines, int start);
void			module_parse(t_game *game, int argc, char **argv);
void			parse_config(t_game *game);
void			parse_map(t_game *game);
t_line_type		set_input_line_type(t_game *game, char *line);

/*
** MLX & Graphics
*/
void			init_game(t_game *game);
void			remember_image(t_game *game, void *ptr);
void			setup_hooks(t_game *game);
void			setup_mlx(t_game *game);
void			load_texture(t_game *game, t_tex *tex);
void			load_wall_textures(t_game *game);
void			put_pixel(int x, int y, int color, t_game *game);

/*
** Raycasting & Movement
*/
void			init_player(t_player *player);
int				key_press(int keycode, void *param);
int				key_release(int keycode, void *param);
void			move_player(t_player *player, t_game *game);
void			init_dda(t_game *game, t_dda *d, float r_dir_x, float r_dir_y);
int				perform_dda(t_game *game, t_dda *d);
t_render_vars	get_render_vars(t_game *game, float r_dir_x, float r_dir_y, \
				int i);
void			render_column(t_game *game, float r_dir_x, float r_dir_y,
					int i);
int				draw_loop(t_game *game);
void			render_minimap(t_game *game);

#endif