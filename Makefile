# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 20:02:16 by weiyang           #+#    #+#              #
#    Updated: 2026/02/17 23:55:38 by yzhang2          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = cub3d

CC        = cc
CFLAGS    = -Wall -Wextra -Werror -Iincludes -Iincludes/minilibx-linux

SRC_DIR   = src
OBJ_DIR   = obj

SRC       = $(shell find $(SRC_DIR) -name "*.c")
OBJ       = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

MLX_DIR   = minilibx-linux
MLX_LIB   = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

# 只有当 libmlx.a 不存在/被清理时，才会触发编译 mlx
$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(MLX_LIB) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(MLX_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(MLX_LIB)

re: fclean all

.PHONY: all clean fclean re
