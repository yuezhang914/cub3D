# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 20:02:16 by weiyang           #+#    #+#              #
#    Updated: 2026/02/18 15:59:11 by yzhang2          ###   ########.fr        #
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

# --- 核心：自动搜集所有 .c 文件 ---
# 使用 wildcard 递归获取子目录下的所有文件
SRCS          = $(wildcard $(SRC_DIR)/*.c) \
                $(wildcard $(SRC_DIR)/*/*.c) \
                $(wildcard $(SRC_DIR)/*/*/*.c)

# 过滤掉属于 bonus 文件夹的文件，作为 Mandatory 的源码
MAND_SRCS     = $(filter-out $(SRC_DIR)/bonus/%, $(SRCS))

# 所有文件都可以参与 Bonus 编译（或者根据需求筛选）
BONUS_SRCS    = $(SRCS)

# 生成对应的 .o 路径
MAND_OBJS     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/mandatory/%.o, $(MAND_SRCS))
BONUS_OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/bonus/%.o, $(BONUS_SRCS))

# --- 规则 ---

all: $(NAME)

# 编译 Mandatory：将对象文件存放在 obj/mandatory
$(NAME): $(MLX_LIB) $(MAND_OBJS)
	@$(CC) $(MAND_OBJS) -o $(NAME) $(MLX_FLAGS)
	@echo "✓ cub3d mandatory ready!"

# 编译 Bonus：将对象文件存放在 obj/bonus
bonus: $(NAME_BONUS)

$(NAME_BONUS): $(MLX_LIB) $(BONUS_OBJS)
	@$(CC) $(BONUS_OBJS) -o $(NAME_BONUS) $(MLX_FLAGS)
	@echo "✓ cub3d bonus ready!"

# 编译规则：Mandatory 和 Bonus 分开存放 .o 以免冲突
$(OBJ_DIR)/mandatory/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/bonus/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -D BONUS -c $< -o $@


$(MLX_LIB):
	@$(MAKE) -s -C $(MLX_DIR)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -s -C $(MLX_DIR) clean

re: fclean all

.PHONY: all clean fclean re bonus