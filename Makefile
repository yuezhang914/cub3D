# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: weiyang <weiyang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 20:02:16 by weiyang           #+#    #+#              #
#    Updated: 2026/02/17 23:55:38 by yzhang2          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME          = cub3d
NAME_BONUS    = cub3d_bonus

CC            = cc
CFLAGS        = -Wall -Wextra -Werror -Iincludes -Iminilibx-linux
MLX_DIR       = minilibx-linux
MLX_LIB       = $(MLX_DIR)/libmlx.a
MLX_FLAGS     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

SRC_DIR       = src
OBJ_DIR       = obj

# --- 1. 共有文件 (Mandatory 和 Bonus 都需要的) ---
COMMON_FILES  = main.c \
                cleanup/clean.c cleanup/error.c \
                parse/parse_config.c parse/parse_entry.c parse/parse_line_kind.c \
                parse/parse_map.c parse/parse_utils.c \
                raycasting/raycasting_cal.c \
                setup/setup.c setup/texture.c \
                utils/ft_readfile.c utils/ft_split.c utils/splitlines.c \
                utils/track_malloc.c utils/utils_simple.c utils/utils_str.c

# --- 2. 必做部分独有文件 ---
MAND_FILES    = render/render_column.c \
                render/draw_loop.c \
                render/mini_map.c \
                game/input.c game/loop.c game/movement.c

# --- 3. Bonus 部分独有文件 (根据你的需求添加) ---
# 注意：即使文件名相同，路径也应区分，或者在编译时通过宏区分
BONUS_FILES   = bonus/render_column_bonus.c \
                bonus/sprite_render_bonus.c \
                bonus/door_bonus.c \
                bonus/mouse_bonus.c \
                render/mini_map.c # 假设小地图只在 bonus 有，或者逻辑通用

# --- 对象文件转换 ---
OBJ_COMMON    = $(addprefix $(OBJ_DIR)/, $(COMMON_FILES:.c=.o))
OBJ_MAND      = $(addprefix $(OBJ_DIR)/, $(MAND_FILES:.c=.o))
OBJ_BONUS     = $(addprefix $(OBJ_DIR)/, $(BONUS_FILES:.c=.o))

all: $(NAME)

# 编译 Mandatory
$(NAME): $(MLX_LIB) $(OBJ_COMMON) $(OBJ_MAND)
	$(CC) $(OBJ_COMMON) $(OBJ_MAND) -o $(NAME) $(MLX_FLAGS)

# 编译 Bonus
bonus: .bonus_flag

.bonus_flag: $(MLX_LIB) $(OBJ_COMMON) $(OBJ_BONUS)
	$(CC) $(OBJ_COMMON) $(OBJ_BONUS) -o $(NAME_BONUS)
	@touch .bonus_flag
	@rm -f .mand_flag

$(MLX_LIB):
	@$(MAKE) -C $(MLX_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f .bonus_flag .mand_flag

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(MLX_DIR) clean

re: fclean all

.PHONY: all clean fclean re bonus