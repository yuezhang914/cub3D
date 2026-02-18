#include "cub3d.h"

/**
 * @brief Bonus 元素的统一处理函数
 * 1. 检查是否存在 Bonus 元素
 * 2. 如果有精灵 'S'，提取它们并抹除地图字符
 * 3. 如果有门 'D'，可以初始化门的状态数组
 */
void    handle_bonus_setup(t_game *game)
{
    if (check_bonus_elements(game)) // 这个函数你已经写在 bonus/utils 里的
    {
        collect_sprites(game);
        // 如果你需要门的状态追踪，可以在这里 init_doors(game);
    }
}