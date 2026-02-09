NAME      = game

CC        = cc
# -Iincludes 告诉编译器，如果找不到头文件，去 includes 目录下找
# -Iincludes/minilibx-linux 同理
CFLAGS    = -Wall -Wextra -Werror -Iincludes -Iincludes/minilibx-linux

SRC_DIR   = src
OBJ_DIR   = obj

# 递归查找 src 目录下所有的 .c 文件
SRC       = $(shell find $(SRC_DIR) -name "*.c")
# 保持目录结构将 .c 映射到 .o
OBJ       = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

MLX_DIR   = includes/minilibx-linux
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(MLX_FLAGS)

# 核心编译规则
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) # 自动创建子目录（如 obj/cleanup, obj/raycasting 等）
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re