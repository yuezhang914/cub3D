#include "cub3d.h"

/**
 * @brief 扫描地图，检查是否存在精灵(S)或门(D)。
 * 这是一个快速扫描，用于决定后续是否分配精灵内存。
 */
bool    check_bonus_elements(t_game *game)
{
    int i;
    int j;

    i = 0;
    while (game->map && game->map[i])
    {
        j = 0;
        while (game->map[i][j])
        {
            if (game->map[i][j] == 'S' || game->map[i][j] == 'D')
                return (true);
            j++;
        }
        i++;
    }
    return (false);
}