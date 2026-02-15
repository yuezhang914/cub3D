/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:36:15 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/11 17:49:15 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H
# define CUBE3D_H

# include "../minilibx-linux/mlx.h"
# include "func.h"

# define W 119 
# define A 97 
# define S 115
# define D 100  
# define ESC 65307
# define PI 3.14159265359

# define WIDTH   1280
# define HEIGHT  720
# define BLOCK 64
# define LEFT 65361
# define RIGHT 65363
# define DEBUG 0
# define BUFFER_SIZE 1024
# define FOV  66.0
# define MINI_HEIGHT 200
# define MINI_WIDTH  200
# define DISTANCE_SEEN 5

typedef struct s_coordinates
{
	float			x;
	float			y;
}	t_coords;

typedef struct s_int_coordinates
{
	int				x;
	int				y;
}	t_int_xy;

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
}	t_line_type;

typedef struct s_img
{
	void			*ptr;
	struct s_img	*next;
}	t_img;

typedef struct s_texture
{
	char			*path;
	void			*img_ptr;
	int				*addr;
	int				width;
	int				height;
	void			*data;
	int				bpp;
	int				size_line;
	int				endian;
}	t_tex;

typedef struct s_gnode
{
	void			*ptr;
	struct s_gnode	*next;
}	t_gnode;

typedef struct s_render_vars
{
	int				x;
	int				start;
	int				end;
	int				line_h;
	int				tex_x;
	int				color;
	t_tex			*tex;
}	t_render_vars;

typedef struct s_player
{
	float			x;
	float			y;
	int				map_x;
	int				map_y;
	float			angle;
	bool			key_up;
	bool			key_down;
	bool			key_left;
	bool			key_right;
	bool			left_rotate;
	bool			right_rotate;
	float			move_speed;
	float			rotate_speed;
}	t_player;

typedef struct s_dda
{
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	float			side_x;
	float			side_y;
	float			delta_x;
	float			delta_y;
	float			perp_dist;
	int				side;
}	t_dda;

typedef struct s_game
{
	t_tex			north;
	t_tex			east;
	t_tex			south;
	t_tex			west;
	t_img			*img_head;
	t_gnode			*track_head;
	int				floor_color;
	int				ceiling_color;
	char			*entire_cubfile;
	char			**cubfile_lines;
	int				cubfile_fd;
	int				map_h;
	int				map_w;
	char			**map;
	void			*mlx;
	void			*win;
	void			*img;
	char			*data;
	int				bpp;
	int				size_line;
	int				endian;
	int				pix_per_unit;
	float			focal_length;
	t_player		player;
}	t_game;

#endif