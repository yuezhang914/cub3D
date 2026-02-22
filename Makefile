# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 20:02:16 by weiyang           #+#    #+#              #
#    Updated: 2026/02/22 19:51:02 by yzhang2          ###   ########.fr        #
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

SRCS          = $(wildcard $(SRC_DIR)/*.c) \
                $(wildcard $(SRC_DIR)/*/*.c) \
                $(wildcard $(SRC_DIR)/*/*/*.c)

MAND_SRCS     = $(filter-out $(SRC_DIR)/bonus/%, $(SRCS))

BONUS_SRCS    = $(SRCS)

MAND_OBJS     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/mandatory/%.o, $(MAND_SRCS))
BONUS_OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/bonus/%.o, $(BONUS_SRCS))

all: $(NAME)

$(NAME): $(MLX_LIB) $(MAND_OBJS)
	@$(CC) $(MAND_OBJS) -o $(NAME) $(MLX_FLAGS)
	@echo "✓ cub3d mandatory ready!"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(MLX_LIB) $(BONUS_OBJS)
	@$(CC) $(BONUS_OBJS) -o $(NAME_BONUS) $(MLX_FLAGS)
	@echo "✓ cub3d bonus ready!"

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
	-@$(MAKE) -s -C $(MLX_DIR) clean

re: fclean all

.PHONY: all clean fclean re bonus